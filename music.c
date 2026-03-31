
/*
*
* Plays the melody throughout gameloop
*
*
*/
#include <osbind.h>
#include "music.h"


/* The song that plays during the loop
*  Note: keyboard clicking sounds play on channel A, disable within atari menu
*        or you are going to override song with key click sounds
*
*/

static const Note song[] = {
    {D4, QUART},
    {D4, QUART},
    {D5, QUART},
    {A4, QUART},
    {AS4, EIGHT},
    {G4, EIGHT},
    {E4, EIGHT},
    {AS4, SIXTEENTH},
    {A4, EIGHT},
    {G4, EIGHT},
    {F4, EIGHT}
};

#define SONG_LENGTH (sizeof(song) / sizeof(song[0]))

/* 
 *  STATE
 */

static int    current_note = 0;
static UINT32 time_on_note = 0;


/* Function purpose: Begins the playing of the song by loading
 *  the data for the first note into the PSG.
 * Input: None
 * Output: None
 * Assumptions: None*/
void start_music(){
    current_note = 0;
    time_on_note = 0;
    set_tone(0, song[0].tuning);
    set_volume(0, 10);
    enable_channel(0, 1, 0);
}


/* Function purpose: Advances the song by time_elapsed ticks.
 *  Called once per game loop tick.
 * Input: Time elapsed
 * Output: Next note
 * Assumptions: None*/
void update_music(UINT32 time_elapsed){
    time_on_note += time_elapsed;

    if (time_on_note >= song[current_note].duration){
        time_on_note = 0;
        current_note++;

        if (current_note >= SONG_LENGTH){
            current_note = 0;
        }

        if (song[current_note].tuning == REST){
            enable_channel(0, 0, 0);
        } else {
            set_tone(0, song[current_note].tuning);
            set_volume(0, 10);
            enable_channel(0, 1, 0);
        }
    }
}