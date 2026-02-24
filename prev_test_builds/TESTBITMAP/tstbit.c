/*
 * tstbit.c - Visual test for bitmaps
 *
 * Screen layout reminder: 640 wide x 400 tall, top-left = (0,0).
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include <osbind.h>
#include "raster.h"

extern UINT8 square_bitmap[8];
extern UINT16 rect_bitmap[16];
extern UINT32 tri_bitmap[32];

/*
 * wait - halts until any key is pressed.
 */
static void wait(void)
{
    Cnecin();
}

int main()
{
    void *base = Physbase();




    /* ----------------------------------------------------------------
     * STAGE 11: plot_bitmaps
     
     * ---------------------------------------------------------------- */

    clear_screen(base);


    pbm8(base, 200, 100, square_bitmap, 8);
    pbm16(base, 200, 200, rect_bitmap, 16);
    wait();
    clear_region(base, 100, 100, 100, 100);
    wait();
    pbm32(base, 300, 256, tri_bitmap, 32);
    wait();
    return 0;
    

}

    
