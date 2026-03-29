/*
*
* Plays the melody throughout gameloop
*
*
*/

#ifndef MUSIC_H
#define MUSIC_H


#include "psg.h"

#define REST    0
/*CS3 == C sharp octave 3, where S means sharp and the following number defines octave*/

/* OCT 3 */
#define C3  1194  #define CS3 1127  #define D3  1064  #define DS3 1004
#define E3   948  #define F3   895  #define FS3  845  #define G3   797
#define GS3  752  #define A3   710  #define AS3  670  #define B3   633

/* OCT 4 */
#define C4   597  #define CS4  564  #define D4   532  #define DS4  502
#define E4   474  #define F4   447  #define FS4  422  #define G4   399
#define GS4  376  #define A4   284  #define AS4  268  #define B4   253

/* OCT 5 */
#define C5   239  #define CS5  225  #define D5   213  #define DS5  201
#define E5   190  #define F5   179  #define FS5  169  #define G5   159
#define GS5  150  #define A5   142  #define AS5  134  #define B5   127

/* OCT 6 */
#define C6   119  #define CS6  113  #define D6   106  #define DS6  100
#define E6    95  #define F6    89  #define FS6   84  #define G6    80
#define GS6   75  #define A6    71  #define AS6   67  #define B6    63

/* Durations */
#define SIX   8    /* sixteenth */
#define EIGHT  15    /* eighth    */
#define QUART  30    /* quarter   */
#define HALF  60    /* half      */
#define WHOLE 120    /* whole     */


void start_music();

void update_music(UINT32 time_elapsed);


#endif /*MUSIC_H*/