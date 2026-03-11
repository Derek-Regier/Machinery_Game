/*
 * tst_rend.c - Visual test driver for render functionality and hypothetical game snapshot.
 *
 * Utilizes init_model to set coordinates for player, health_bar, item, enemy
 * and boss objects by calling upon render functions, displaying to screen. 
 * 
 * Press any key to advance through each render and gamesnapshot.
 * 
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include <osbind.h>
#include "render.h"
#include "model.h"
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
    Model model;

    init_model(&model);

    /* STAGE 1: render player */
   
    render_player(&model.player, base);
    wait();

    /* STAGE 2: render enemy */
    
    render_enemy(&model.enemy[0], base);
    wait();

    /* STAGE 3: render boss */
    
    render_boss(&model.boss, base);
    wait();

    /* STAGE 4: render item */
    
    render_item(&model.item[0], base);
    wait();

    /* STAGE 5: render health bar */
    
    render_healthbar(&model.healthbar, base);
    wait();

    /* STAGE 6: full render */
  
    render(&model, base);
    wait();

    clear_screen(base);
    return 0;
}