/*
 * isr.c
 *
 * Installs and uninstalls interrupt vectors for the VBL and IKBD ISRs.
 * Provides the C-level VBL ISR body (do_VBL_ISR).
 * The IKBD ISR body (do_IKBD_ISR) lives in input.c so that module
 * owns all hardware keyboard/mouse state.
 *
 * Authors: Derek Regier, Aydin Saloniu, Chintan Thakor
 * Course:  COMP 2659, Winter 2026
 */

#include "isr.h"
#include <osbind.h>

#define VBL_VECTOR  28
#define IKBD_VECTOR 70

/* Assembly functions declared in ISR_ASM.S */
extern void vbl_isr();
extern void ikbd_isr();

/* Saved originals so we can restore them on exit */
static Vector orig_VBL;
static Vector orig_IKBD;

/* Polled by the main loop to know when a new frame is ready */
volatile int render_request = 0;

/*
 * Overwrites interrupt vector 'num' with 'vec'.
 * Temporarily enters supervisor mode to access the vector table.
 * Returns the previous vector stored at that slot.
 */
Vector install_vector(int num, Vector vec)
{
    Vector  orig;
    Vector *vptr = (Vector *)((long)num << 2);
    long old_ssp = Super(0);

    orig = *vptr;
    *vptr = vec;
    Super(old_ssp);

    return orig;
}

/*
 * Installs the VBL and IKBD ISR trampolines and disables the
 * MIDI 6850 receive interrupt.  Must be called before the game loop.
 */
void install_vectors(void)
{
    long old_ssp;
    volatile UINT8 *midi_ctrl = (volatile UINT8 *)0xFFFC04;

    orig_VBL = install_vector(VBL_VECTOR,  vbl_isr);
    orig_IKBD = install_vector(IKBD_VECTOR, ikbd_isr);

    /* Clear bit 7 (RIE) of the MIDI 6850 control register */
    old_ssp = Super(0);
    *midi_ctrl = 0x16;   /* divide/64, 8N1, RTS low, RIE disabled */
    Super(old_ssp);
}

/*
 * Restores the original VBL and IKBD vectors and re-enables the MIDI
 * receive interrupt.  Must be called before the program exits.
 */
void uninstall_vectors(void)
{
    long old_ssp;
    volatile UINT8 *midi_ctrl = (volatile UINT8 *)0xFFFC04;

    install_vector(VBL_VECTOR,  orig_VBL);
    install_vector(IKBD_VECTOR, orig_IKBD);

    /* Restore bit 7 (RIE) of the MIDI 6850 control register */
    old_ssp = Super(0);
    *midi_ctrl = 0x96;   /* divide/64, 8N1, RTS low, RIE enabled */
    Super(old_ssp);
}

/*
 * Sets render_request so the main loop knows a new frame period began.
 */
void do_VBL_ISR(void)
{
    render_request = 1;
}