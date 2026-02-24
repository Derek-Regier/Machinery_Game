/*
 * tstfont.c - Visual test driver for character plotting and string plotting.
 *
 * Press any key to advance through each test stage.
 * 
 * Screen layout reminder: 640 wide x 400 tall, top-left = (0,0).
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */


#include <osbind.h>
#include "raster.h"
#include "font.h"

static void wait(void)
{
    Cnecin();
}

int main()
{

    
    UINT8 *base = Physbase();

    clear_screen((UINT32 *)base);
    /* each character is 8 pixels wide*/
    plot_character(base, 0, 0, 'A', font);
    plot_character(base, 0, 8, 'B', font);

    plot_string(base, 50, 50, "SUPER COOL STRING n!", font);
    
    wait();

    return 0;
}
