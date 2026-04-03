/*
 * isr.h
 *
 * Interface for interrupt vector installation and the VBL ISR body.
 * install_vectors() / uninstall_vectors() must be called at the
 * start and end of main() so the OS state is correctly restored.
 *
 * Authors: Derek Regier, Aydin Saloniu, Chintan Thakor
 * Course:  COMP 2659, Winter 2026
 */

#ifndef ISR_H
#define ISR_H

#include "TYPES.H"

/* Function pointer type matching the 68000 ISR calling convention. */
typedef void (*Vector)(void);

/*
 * Overwrites interrupt vector 'num' with 'vec'.
 * Returns the original vector so it can be restored later.
 * Runs in supervisor mode internally.
 */
Vector install_vector(int num, Vector vec);

/*
 * Installs the VBL and IKBD ISR trampolines.
 * Also disables the MIDI 6850 receive interrupt to prevent
 * spurious IKBD vector triggers from MIDI activity.
 * Must be called before the main game loop.
 */
void install_vectors(void);

/*
 * Restores the original VBL and IKBD vectors saved by install_vectors().
 * Re-enables the MIDI receive interrupt.
 * Must be called before the program exits.
 */
void uninstall_vectors(void);

/*
 * C body of the VBL ISR. Called from the _vbl_isr trampoline.
 * Sets the render_request flag that the main loop polls.
 */
void do_VBL_ISR(void);

/*
 * Flag set to 1 each VBL; cleared by the main loop after rendering.
 * Declared volatile so the compiler never caches it in a register.
 */
extern volatile int render_request;

#endif /* ISR_H */
