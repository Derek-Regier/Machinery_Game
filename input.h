/*
 * input.h
 * 
 * Hardware-independent middle-layer input module for keyboard polling.
 * Wraps TOS system calls (Cconis, Cnecin) so the main game module
 * contains zero OS-specific input code.
 *
 * Authors: Derek Regier, Aydin, Chintan Thakor
 * COMP 2659 - Winter 2026
 */

#ifndef INPUT_H
#define INPUT_H

#include "TYPES.H"

/* Atari ST IKBD scan codes for game keys*/
#define SCAN_W 0x11
#define SCAN_A 0x1E
#define SCAN_S 0x1F
#define SCAN_D 0x20
#define SCAN_E 0x12
#define SCAN_J 0x24
#define SCAN_L 0x26
#define SCAN_ESC 0x01

/* ISR entry point — called from _ikbd_isr in ISR_ASM.S.
 * Reads one byte from the 6850 and updates key state or mouse state.
 * Must NOT be called from non-ISR code. */
void do_IKBD_ISR(void);

/* Key state query.
 * Returns 1 if the key with the given scan code is currently held
 * down (make received, break not yet received), 0 otherwise.
 * Safe to call from non-ISR code.*/
int is_key_held(UINT8 scan);

/* Circular buffer dequeue.
 * Returns the scan code of the next discrete key-press event, or 0
 * if the buffer is empty.  Each call to keystroke() consumes one entry.
 * Uses mask_ikbd / unmask_ikbd internally for the critical section. */
UINT8 keystroke(void);

/*
 * Returns 1 if the circular buffer contains at least one pending
 * keystroke, 0 if empty.  Safe to call at any rate — no ACIA access.
 * Use this to guard keystroke() calls in tight loops.
 */
int has_keystroke(void);

/* Mouse accessors.
 * All three are safe to call from non-ISR code; they protect the read
 * with a brief IKBD mask so values are never partially updated.*/
int get_mouse_x(void);
int get_mouse_y(void);
int get_mouse_button(void);

/* Mask/unmask the IKBD interrupt at the 6850 level (not the CPU IPL)
 * so access to shared input state is protected with minimal overhead. */
void mask_ikbd(void);
void unmask_ikbd(void);
/*
 * Returns 1 if there is a keypress waiting to be read, 0 otherwise.
 * Does NOT consume the input.
 */
int has_input(void);

/*
 * Reads and returns the next pending character from the keyboard.
 * Assumes has_input() returned true before calling this.
 * Will block if called with no pending input.
 */
char get_input(void);

#endif