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
#include "font.h"

extern UINT8 square_bitmap[8];
extern UINT16 rect_bitmap[16];
extern UINT32 tri_bitmap[32];
extern UINT32 player[64];
extern UINT32 health_bar[12];
extern UINT32 enemy[64];
extern UINT32 boss[128][4];
extern UINT16 hp_pot[16];
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

    int r, w;


    /* ----------------------------------------------------------------
     * STAGE 11: plot_bitmaps
     
     * ---------------------------------------------------------------- */

    clear_screen(base);
    wait();
    
    pbm32(base, 4, 4, health_bar, 12);
    wait();
    pbm32(base, 4, 36, health_bar, 12);
    wait();
    pbm32(base, 300, 256, player, 64);
    wait();
    pbm32(base, 300, 500, enemy, 64);
    wait();
    pbm16(base, 300, 200, hp_pot, 16);
    wait();
    for (r = 0; r < 128; r++){
        for (w = 0; w < 4; w++){
            pbm32(base, 100 + r, 200 + (w * 32), &boss[r][w], 1);}
        }
    wait();
    clear_screen(base);
    
    
    wait();
    return 0;
    

}

    
