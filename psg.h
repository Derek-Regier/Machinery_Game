/*
* Low-level PSG library 
*
*/

#ifndef PSG_H
#define PSG_H

#include <osbind.h>
#include "types.h"



void write_psg(int reg, UINT8 val);

UINT8 read_psg(int reg);

void set_tone(int channel, int tuning);

void set_volume(int channel, int volume);

void enable_channel(int channel, int tone_on, int noise_on);

void stop_sound();

#endif