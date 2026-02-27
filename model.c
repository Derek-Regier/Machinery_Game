/*
 * model.c - Game world model initialization.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "model.h"

/*
 * Fills *model with the game's initial state:
 *   Player centred left-side of walkable area, full health.
 *   One enemy placed to the right of player.
 *   Boss placed far right, full health, not yet summoned.
 *   One health potion in inventory.
 *   Note: Initializaing with {} braces does not compile.
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
    model->player.h = 32;
    model->player.w = 32;

    model->enemy[0].x = 400;
    model->enemy[0].y = 200;
    model->enemy[0].delta_x = 0;
    model->enemy[0].delta_y = 0;
    model->enemy[0].health = 50;
    model->enemy[0].damage = 8;
    model->enemy[0].w = 32;
    model->enemy[0].h = 32;
    model->enemy[0].is_attacking = FALSE;

    model->boss.x = 580;
    model->boss.y = 200;
    model->boss.delta_x = 0;
    model->boss.delta_y = 0;
    model->boss.health = 200;
    model->boss.max_health = 200;
    model->boss.damage = 15;
    model->boss.summoned = FALSE;
    model->boss.w = 48;
    model->boss.h = 48;

    model->item[0].x = 0;
    model->item[0].y = 0;
    model->item[0].h = 16;
    model->item[0].w = 16;
    model->item[0].value = 30;
}
