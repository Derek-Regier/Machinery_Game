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