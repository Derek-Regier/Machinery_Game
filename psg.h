/*
* low-level PSG library 
*
*/

#ifndef PSG_H
#define PSG_H
/*120 bpm == whole note*/
#define WHOLE   120
#define HALF     60
#define QUARTER  30
#define EIGHTH   15
#define SIXTEENTH 7

#include "types.h"

extern volatile UINT8 curr_r7_val;

void write_psg(int reg, UINT8 val);

UINT8 read_psg(int reg);

void set_tone(int channel, int tuning);

void set_volume(int channel, int volume);

void enable_channel(int channel, int tone_on, int noise_on);

void set_noise(int tuning);

void set_envelope(int shape, unsigned int sustain);

void stop_sound();

#endif