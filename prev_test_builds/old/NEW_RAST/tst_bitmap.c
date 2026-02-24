/*
 * tst_rast.c - Visual test driver for the raster library.
 *
 * Each test draws to a distinct region of the screen.
 * Press any key to advance through each test stage.
 * Excludes bitmap, character, and string tests (tested separately).
 *
 * Screen layout reminder: 640 wide x 400 tall, top-left = (0,0).
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include <osbind.h>
#include "raster.h"

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
     * STAGE 11: plot_bitmap_8 
     
                void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col,
                   const UINT8 *bitmap, UINT16 height);
     * ---------------------------------------------------------------- */

    clear_screen(base);


    plot_bitmap_8(base, 100, 100, bullet_vitmaps[o], 16);
    wait();



    }
