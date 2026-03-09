

/*Renamed file to castle_crashers.c*/
#include "crashers.h"


UINT8 screenBuffer[32255]; /* allocated 32,000 Byte Buffer 256-Byte Alligned*/

UINT32 get_time(){
    long old_ssp;
    old_ssp = Super(0); /* enter privileged mode */
    timeNow = *timer;
    Super(old_ssp); /* exit privileged mode as soon as possible */
}

UINT32 timeThen, timeNow, timeElapsed;

timeNow = get_time();
timeElapsed = timeNow - timeThen;
if (timeElapsed > 0) {
    /* trigger synchronous events based on timeElapsed */
    /* render model */
    timeThen = timeNow;
}



