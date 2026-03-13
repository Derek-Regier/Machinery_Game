/*
 * Game world model initialization.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "model.h"

/*
 *   Function Purpose: Fills *model with the game's initial state:
 *   Player centred left-side of walkable area, full health.
 *   One enemy placed to the right of player.
 *   Boss placed far right, full health, not yet summoned.
 *   One health potion in inventory.
 *   Note: Initializaing with {} braces does not compile.
 *
 * Input: The model
 * Output: None, modifies current game state
 * Assumptions: All valid declarations
 */
void init_model(Model *model)
{
    int i;

    /* Player init */
    model->player.x = 64;
    model->player.y = 200;
    model->player.delta_x = 0;
    model->player.delta_y = 0;
    model->player.health = 100;
    model->player.damage = 10;
    model->player.is_attacking  = FALSE;
    model->player.item_cooldown  = 0;
    model->player.attack_cooldown = 0;
    model->player.h = 64;
    model->player.w = 32;

    /* Health bar init */
    model->healthbar.x = 4;
    model->healthbar.y = 4;
    model->healthbar.h = 12;
    model->healthbar.w = model->player.health;
    model->healthbar.value = model->player.health;

    /* Enemies init
     * Indices 0-1 are the two start enemies; active from the start.
     * All remaining slots start inactive; spawn_enemy activates them. */
    model->enemy[0].x = 350;
    model->enemy[0].y = 200;
    model->enemy[0].delta_x = 0;
    model->enemy[0].delta_y = 0;
    model->enemy[0].health = 50;
    model->enemy[0].damage = 8;
    model->enemy[0].w = 32;
    model->enemy[0].h = 64;
    model->enemy[0].is_attacking = TRUE;
    model->enemy[0].active = TRUE;

    model->enemy[1].x = 500;
    model->enemy[1].y = 200;
    model->enemy[1].delta_x = 0;
    model->enemy[1].delta_y = 0;
    model->enemy[1].health = 50;
    model->enemy[1].damage = 8;
    model->enemy[1].w = 32;
    model->enemy[1].h = 64;
    model->enemy[1].is_attacking = FALSE;
    model->enemy[1].active = TRUE;

    for (i = 2; i < MAX_ENEMIES; i++)
    {
        model->enemy[i].x = 0;
        model->enemy[i].y = 0;
        model->enemy[i].delta_x = 0;
        model->enemy[i].delta_y = 0;
        model->enemy[i].health = 0;
        model->enemy[i].damage = 0;
        model->enemy[i].w = 0;
        model->enemy[i].h = 0;
        model->enemy[i].is_attacking = FALSE;
        model->enemy[i].active = FALSE;
    }

    model->enemy_count = 2; /* only the two tutorial enemies are live */

    /* Boss init */
    model->boss.x = 500;
    model->boss.y = 150;
    model->boss.delta_x = 0;
    model->boss.delta_y = 0;
    model->boss.health = 200;
    model->boss.max_health = 200;
    model->boss.damage = 15;
    model->boss.summoned = FALSE;
    model->boss.is_attacking = FALSE;
    model->boss.active = FALSE; /* activated when stage reaches 4 */
    model->boss.w = 128;
    model->boss.h = 128;

    /* create items
     * All four items start off-screen (x=700 is past the right edge).
     * drop_item() moves item[stage] to a visible position when a wave clears. */
    for (i = 0; i < NUM_ITEMS; i++)
    {
        model->item[i].x = 700;
        model->item[i].y = 200;
        model->item[i].h = 16;
        model->item[i].w = 16;
        model->item[i].value = 30;
    }

    /* Game state  */
    model->stage = 0;
    model->quit = FALSE;
}
void reset_movement(Model *model){
    int i;
    model->player.delta_x = 0;
    model->player.delta_y = 0;
    for (i = 0; i < MAX_ENEMIES; i++){
        if (model->enemy[i].active){
            model->enemy[i].delta_x = 0;
            model->enemy[i].delta_y = 0;
        }
    }
    /*Is active check first but will do that once ready*/
    model->boss.delta_x = 0;
    model->boss.delta_y = 0;
}
