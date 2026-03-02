/*
 * Definition of various data types
 * Unsigned intergers of various lengths to work with the Atari
 * Boolean expressions as stdbool is not a valid import for the Atari
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
*/


#ifndef TYPES_H
#define TYPES_H

typedef unsigned char UINT8;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;

typedef char bool;
#define TRUE  1
#define FALSE 0

#endif
