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