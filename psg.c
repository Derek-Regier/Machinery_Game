/*
* Low-level PSG Library 
*/ 

#include <osbind.h>
#include "psg.h"


volatile char *PSG_reg_select = 0xFF8800;
volatile char *PSG_reg_write  = 0xFF8802;




/*Function: ***** ADD MORE DESCRIPTION TO ALL FUNCTIONS *****
Writes the given byte value (0-255) to 
the given PSG register (0-15).
This is a helper
routine to be used by the 
other functions in this module.*/

void write_psg(int reg, UINT8 val){
    long old_ssp = Super(0);

    *PSG_reg_select = reg;
    *PSG_reg_write = val;

    Super(old_ssp);

}

/*
Used for testing sound 
reads value from psg resgister and returns it*/
UINT8 read_psg(int reg){
    UINT8 prev_psg:

    long old_ssp = Super(0);
    *PSG_reg_select = reg;
    prev_psg = *PSG_reg_write;

    Super(old_ssp);

    return prev_psg;
}


/*Loads the tone registers (coarse and fine) for 
the given channel (0=A, 1=B, 2=C) with the
given 12-bit tuning.*/
void set_tone(int channel, int tuning){
    write_psg(channel, tuning);

}

/*Loads the volume register for the given channel.*/
void set_volume(int channel, int volume){
    if (volume >= 15 || volume <=0){
        return;
    }
    if(channel == 0){
        write_psg(8, volume);
    }else if (channel == 1){
        write_psg(9, volume);
    }else if (channel == 2){
        write_psg(10, volume);
    }else{
        return;
    }
}

/*Turns the given channel’s tone/noise signals 
on/off (0=off, 1=on).*/
void enable_channel(int channel, int tone_on, int noise_on){
    long old_ssp = Super(0);
    *PSG_reg_select = channel;
    *PSG_reg
    Super(old_ssp);
}
/*Silences all PSG sound production.*/
void stop_sound(){
    long old_ssp = Super(0);

    write_psg(8, 0); /* set channel A volume = 0 */
    write_psg(9, 0); /* set channel B volume = 0 */
    write_psg(10, 0); /* set channel C volume = 0 */

    Super(old_ssp);
}



