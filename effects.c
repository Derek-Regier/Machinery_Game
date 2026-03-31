/*Sound effects for game*/

#include "effects.h"




void attack_swing_sound(){
    set_tone(1, C4);          
    set_envelope(15, 800); 
    write_psg(9, 0x10);
    enable_channel(1, 1, 0);

}

void on_hit_sound(){
    set_tone(1, FS5);          
    set_envelope(9, 800);     
    write_psg(9, 0x10);        
    enable_channel(1, 1, 0);

}

void enemy_death_sound(){
    set_noise(16);
    set_envelope(15, 800); 
    write_psg(10, 0x10);
    enable_channel(2, 0, 1);

}


void item_pickup_sound(){
    set_tone(1,GS6);
    set_envelope(9, 200);
    write_psg(9, 0x10);
    enable_channel(1,1,0);
}

