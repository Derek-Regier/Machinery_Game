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
    model->player.x = 64;
    model->player.y = 200;
    model->player.delta_x = 0;
    model->player.delta_y = 0;
    model->player.health = 100;
    model->player.damage = 10;
    model->player.is_attacking = FALSE;
    model->player.item_cooldown = 0;
    model->player.attack_cooldown = 0;
    model->player.h = 64;
    model->player.w = 32;

    model->healthbar.x = 4;
    model->healthbar.y = 4;
    model->healthbar.h = 12;
    model->healthbar.w = model->player.health;
    model->healthbar.value = model->player.health;

    model->enemy[0].x = 400;
    model->enemy[0].y = 200;
    model->enemy[0].delta_x = 0;
    model->enemy[0].delta_y = 0;
    model->enemy[0].health = 50;
    model->enemy[0].damage = 8;
    model->enemy[0].w = 32;
    model->enemy[0].h = 64;
    model->enemy[0].is_attacking = FALSE;

    model->boss.x = 500;
    model->boss.y = 200;
    model->boss.delta_x = 0;
    model->boss.delta_y = 0;
    model->boss.health = 200;
    model->boss.max_health = 200;
    model->boss.damage = 15;
    model->boss.summoned = FALSE;
    model->boss.w = 128;
    model->boss.h = 128;

    model->item[0].x = 128;
    model->item[0].y = 4;
    model->item[0].h = 16;
    model->item[0].w = 16;
    model->item[0].value = 30;

    model->quit = FALSE;

    
}
