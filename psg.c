/*
* Low-level PSG Library 
*/ 

#include <osbind.h>
#include "psg.h"


volatile char *PSG_reg_select = 0xFF8800;
volatile char *PSG_reg_write  = 0xFF8802;

long old_ssp = Super(0);


/*Function: ***** ADD MORE DESCRIPTION TO ALL FUNCTIONS *****
Writes the given byte value (0-255) to 
the given PSG register (0-15).
This is a helper
routine to be used by the 
other functions in this module.*/

void write_psg(int reg, UINT8 val){

    if((val >= 256 || val < 0) || (reg >= 16 || reg < 0) ){
        return;
    }

}

UINT8 read_psg(int reg){

}


/*Loads the tone registers (coarse and fine) for 
the given channel (0=A, 1=B, 2=C) with the
given 12-bit tuning.*/
void set_tone(int channel, int tuning){

    *PSG_reg_select = channel;
    *PSG_reg_write = tuning;


}

/*Loads the volume register for the given channel.*/
void set_volume(int channel, int volume){

    *PSG_reg_select = channel;
    *PSG_reg_write = volume;


}
/*Turns the given channel’s tone/noise signals 
on/off (0=off, 1=on).*/
void enable_channel(int channel, int tone_on, int noise_on){
    *PSG_reg_select = channel;
    *PSG_reg

}
/*Silences all PSG sound production.*/
void stop_sound(){

    *PSG_reg_select = 8;		/* set channel A volume = 0 */
	*PSG_reg_write  = 0;

    *PSG_reg_select = 9;		/* set channel B volume = 0 */
	*PSG_reg_write  = 0;


    *PSG_reg_select = 10;		/* set channel C volume = 0 */
	*PSG_reg_write  = 0;

}



Super(old_ssp);