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
    model->player.y = 316; /* center of walkable lane (264-368) */
    model->player.delta_x = 0;
    model->player.delta_y = 0;
    model->player.health = 100;
    model->player.damage = 8;
    model->player.potions = 4;
    model->player.is_attacking = FALSE;
    model->player.item_cooldown = 0;
    model->player.attack_cooldown = 0;
    model->player.dash_cooldown = 0;
    model->player.trail_x = 0;
    model->player.trail_y = 0;
    model->player.trail_facing = 1;
    model->player.trail_timer = 0;
    model->player.hit_flash_timer = 0;
    model->player.hit_facing = 0;
    model->player.h = 64;
    model->player.w = 32;
    model->player.facing = 1;
    model->player.anim_frame = 0;
    model->player.anim_counter = 0;
    model->player.potions = 0;
 

    /* Health bar init */
    model->healthbar.x = 4;
    model->healthbar.y = 4;
    model->healthbar.h = 12;
    model->healthbar.w = model->player.health;
    model->healthbar.value = model->player.health;

    /* Enemies init
     * Indices 0-1 are the two start enemies; active from the start.
     * All remaining slots start inactive; spawn_enemy activates them. */
    model->enemy[0].x = 530;
    model->enemy[0].y = 296; /* upper part of walkable lane */
    model->enemy[0].delta_x = 0;
    model->enemy[0].delta_y = 0;
    model->enemy[0].health = 20;
    model->enemy[0].damage = 4;
    model->enemy[0].w = 32;
    model->enemy[0].h = 64;
    model->enemy[0].is_attacking = FALSE;
    model->enemy[0].active = TRUE;
    model->enemy[0].attack_cooldown = 0;
    model->enemy[0].attack_windup = 0;
    model->enemy[0].y_offset = -20; /* targets 20px above player — breaks column stacking */
    model->enemy[0].facing = -1; 
    model->enemy[0].anim_frame = 0;
    model->enemy[0].anim_counter = 0;
    model->enemy[0].hit_flash_timer = 0;
    model->enemy[0].target_x = 64;  /* player start x */
    model->enemy[0].target_y = 316; /* player start y */
    model->enemy[0].move_timer = 0; /* refresh on first tick */
    model->enemy[1].y = 336; /* lower part of walkable lane */
    model->enemy[1].delta_x = 0;
    model->enemy[1].delta_y = 0;
    model->enemy[1].health = 20;
    model->enemy[1].damage = 4;
    model->enemy[1].w = 32;
    model->enemy[1].h = 64;
    model->enemy[1].is_attacking = FALSE;
    model->enemy[1].active = TRUE;
    model->enemy[1].attack_cooldown = 0;
    model->enemy[1].attack_windup = 0;
    model->enemy[1].y_offset = 20; /* targets 20px below player — ensures different path */
    model->enemy[1].facing = -1;
    model->enemy[1].anim_frame = 0;
    model->enemy[1].anim_counter = 0;
    model->enemy[1].hit_flash_timer = 0;
    model->enemy[1].target_x = 64;  /* player start x */
    model->enemy[1].target_y = 316; /* player start y */
    model->enemy[1].move_timer = 0; /* refresh on first tick */

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
        model->enemy[i].attack_cooldown = 0;
        model->enemy[i].attack_windup = 0;
        model->enemy[i].y_offset = 0;
        model->enemy[i].facing = -1;
        model->enemy[i].anim_frame = 0;
        model->enemy[i].anim_counter = 0;
        model->enemy[i].hit_flash_timer = 0;
        model->enemy[i].target_x = 0;
        model->enemy[i].target_y = 0;
        model->enemy[i].move_timer = 0;
    }

    model->enemy_count = 2; /* only the two tutorial enemies are live */

    /* Boss init */
    model->boss.x = 500;
    model->boss.y = 240; /* boss is 128px tall; y=240 puts bottom at 368 = MAX_Y */
    model->boss.delta_x = 0;
    model->boss.delta_y = 0;
    model->boss.health = 200;
    model->boss.max_health = 200;
    model->boss.damage = 8;
    model->boss.summoned = FALSE;
    model->boss.is_attacking = FALSE;
    model->boss.active = FALSE; /* activated when stage reaches 4 */
    model->boss.w = 128;
    model->boss.h = 128;
    model->boss.attack_cooldown = 0;
    model->boss.hit_flash_timer = 0;
    model->boss.stomp_frame = 0;
    model->boss.facing = -1;
    model->boss.anim_frame = 0;
    model->boss.anim_counter = 0;
    model->boss.target_x = 64;  /* player start x */
    model->boss.target_y = 316; /* player start y */
    model->boss.move_timer = 0; /* refresh on first tick */

    /* create items
     * All four items start off-screen (x=700 is past the right edge).
     * drop_item() moves item[stage] to a visible position when a wave clears. */
    for (i = 0; i < NUM_ITEMS; i++)
    {
        model->item[i].x = 700;
        model->item[i].y = 316; /* off-screen but within lane height */
        model->item[i].h = 16;
        model->item[i].w = 16;
        model->item[i].value = 30;
        model->item[i].grabbed = TRUE; /* inert until drop_item activates it */
    }

    /* Game state  */
    model->stage = 0;
    model->spawn_start = 0;
    model->spawn_end   = 0;
    model->spawn_timer = 0;
    model->quit = FALSE;
}

/* Unused function, intended to work with continous input/key releases */
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