#include <osbind.h>

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