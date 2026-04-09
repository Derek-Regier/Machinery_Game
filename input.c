/*
 * input.c
 *
 * Implementation of the hardware-independent input module.
 * This is the ONLY file that may call TOS keyboard system calls.
 * All other modules must use has_input() and get_input() instead.
 *
 * Authors: Derek Regier, Aydin, Chintan Thakor
 * COMP 2659 - Winter 2026
 */

#include "input.h"
#include <osbind.h>


/* 6850 ACIA register addresses */
#define IKBD_ACIA ((volatile UINT8 *)0xFFFC00)  /* ctrl (W) / status (R) */
#define IKBD_DATA ((volatile UINT8 *)0xFFFC02)  /* receive data register  */

/* 6850 control words */
#define RIE_ON 0x96   /* divide/64, 8N1, RTS low, RIE enabled  */
#define RIE_OFF 0x16   /* same configuration but RIE disabled    */

/* RDRF flag in the 6850 status register */
#define RDRF_BIT 0x01

/* Mouse header bytes are in the range 0xF8-0xFF */
#define MOUSE_HDR 0xF8

/* Circular keyboard buffer size (must be a power of 2) */
#define KB_BUF_SZ  256
#define KB_BUF_MSK (KB_BUF_SZ - 1)

static volatile UINT8 key_down[128];    /* 1 = key currently held        */

static volatile UINT8 kb_buf[KB_BUF_SZ]; /* scan-code circular buffer    */
static volatile int kb_head = 0; /* next dequeue index            */
static volatile int kb_tail = 0;  /* next enqueue index            */
static volatile int kb_fill = 0; /* number of entries in buffer   */

static volatile int mouse_x = 0;   /* absolute x, clamped to screen */
static volatile int mouse_y = 0;   /* absolute y, clamped to screen */
static volatile int mouse_btn = 0;   /* 1 = left button held          */
static volatile int mouse_prog = 0;   /* bytes received so far (0-2)   */
static volatile UINT8 mouse_buf[3];     /* raw bytes of current packet   */

/*
 * Appends 'scan' to the circular scan-code buffer.
 * Silently drops the byte if the buffer is full.
 */
static void enqueue_scan(UINT8 scan)
{
    if (kb_fill < KB_BUF_SZ)
    {
        kb_buf[kb_tail] = scan;
        kb_tail = (kb_tail + 1) & KB_BUF_MSK;
        kb_fill++;
    }
}

/*
 * Processes the next byte of an in-progress mouse packet.
 * mouse_prog tracks which byte we expect next (0=header, 1=dx, 2=dy).
 * When the third byte arrives the global coordinates are updated.
 * Input:  byte - raw byte from the 6850
 */
static void handle_mouse_byte(UINT8 byte)
{
    int delta_x;
    int delta_y;

    mouse_buf[mouse_prog] = byte;
    mouse_prog++;

    if (mouse_prog < 3)
        return;

    /* All three bytes received; update mouse state */
    delta_x = (signed char)mouse_buf[1];
    delta_y = (signed char)mouse_buf[2];

    mouse_x += delta_x;
    mouse_y += delta_y;

    if (mouse_x > 632) mouse_x = 632;
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y > 392) mouse_y = 392;
    if (mouse_y < 0) mouse_y = 0;

    mouse_btn  = (mouse_buf[0] & 0x02) ? 1 : 0;
    mouse_prog = 0;
}
/*
 * Called from the _ikbd_isr assembly trampoline.
 * Reads one byte from the 6850 and routes it to the correct handler.
 * Disables RIE at entry to prevent re-entrant IKBD interrupts;
 * re-enables it before returning.
 */
void do_IKBD_ISR(void)
{
    UINT8 byte;
    UINT8 scan;

    *IKBD_ACIA = RIE_OFF;

    if (*IKBD_ACIA & RDRF_BIT)
    {
        byte = *IKBD_DATA;

        if (mouse_prog > 0)
        {
            /* Continuation byte of an in-progress mouse packet */
            handle_mouse_byte(byte);
        }
        else if (byte >= MOUSE_HDR)
        {
            /* First byte of a new mouse packet */
            handle_mouse_byte(byte);
        }
        else if (byte & 0x80)
        {
            /* Break code: strip the high bit to get the scan code */
            scan = byte & 0x7F;
            key_down[scan] = 0;
        }
        else
        {
            /* Make code: mark key held and enqueue for discrete events */
            key_down[byte] = 1;
            enqueue_scan(byte);
        }
    }

    *IKBD_ACIA = RIE_ON;
}

/*
 * Returns 1 if the key identified by 'scan' is currently held down,
 * 0 otherwise.  The result reflects the live key_down[] table.
 */
int is_key_held(UINT8 scan)
{
    return (scan < 128) ? (int)key_down[scan] : 0;
}

/*
 * Returns 1 if the circular buffer contains at least one pending keystroke,
 * 0 otherwise.  No ACIA access — safe to call at any rate from any context.
 */
int has_keystroke(void)
{
    return kb_fill > 0;
}

/*
 * Dequeues and returns the next scan code from the circular buffer.
 * Returns 0 if the buffer is empty.
 * Protects the read with a brief IKBD mask to avoid torn reads.
 */
UINT8 keystroke(void)
{
    UINT8 scan;

    mask_ikbd();
    if (kb_fill > 0)
    {
        scan = kb_buf[kb_head];
        kb_head = (kb_head + 1) & KB_BUF_MSK;
        kb_fill--;
    }
    else
    {
        scan = 0;
    }
    unmask_ikbd();

    return scan;
}

/*
 * Returns the current absolute mouse X coordinate (0-632).
 * Protected by a brief IKBD mask so the value is never half-updated.
 */
int get_mouse_x(void)
{
    int val;
    mask_ikbd();
    val = mouse_x;
    unmask_ikbd();
    return val;
}

/*
 * Returns the current absolute mouse Y coordinate (0-392).
 */
int get_mouse_y(void)
{
    int val;
    mask_ikbd();
    val = mouse_y;
    unmask_ikbd();
    return val;
}

/*
 * Returns 1 if the left mouse button is currently held, 0 otherwise.
 */
int get_mouse_button(void)
{
    int val;
    mask_ikbd();
    val = mouse_btn;
    unmask_ikbd();
    return val;
}

/*
 * Disables the IKBD receive interrupt at the 6850 level.
 * Call before accessing shared input state in non-ISR code.
 */
void mask_ikbd(void)
{
    *IKBD_ACIA = RIE_OFF;
}

void unmask_ikbd(void)
{
    *IKBD_ACIA = RIE_ON;
}
/*
 * Returns 1 if a keypress is waiting in the TOS input buffer.
 * Wraps the Cconis TOS system call.
 */
int has_input(void)
{
    return Cconis() != 0;
}

/*
 * Reads and returns the next character from the keyboard buffer.
 * Wraps Cnecin - reads without echoing to screen.
 * Caller must check has_input() first to avoid blocking.
 */
char get_input(void)
{
    return (char)Cnecin();
}