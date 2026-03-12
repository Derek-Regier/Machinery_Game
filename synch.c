/*
 * Synch events implementation
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "synch.h"
#define MAX_X 608 /* 640 pixels - 32 pixels for player */ 
#define MAX_Y 368 /*400 - 32*/
#define ENGAGE_RANGE_X 32   /* pixels - touching player position bitmap */
#define ENGAGE_RANGE_Y 16   /* pixels - vertical forgiveness */

/* used for signed difference for enemy movement logic*/
static int diff(int a, int b)
{
    return a - b;
}

/*
 * Sets enemy delta_x and delta_y to move toward the player,
 * but stops on each axis independently once inside engage range.
 * Enemy will not move on an axis it is already close enough on.
 *
 * Input:  enemy  - the enemy to update velocity for
 *         player - the player to move toward
 * Output: modifies enemy->delta_x and enemy->delta_y only
 */
void update_enemy_velocity(Enemy *enemy, const Player *player)
{
    int dx = diff((int)player->x, (int)enemy->x);
    int dy = diff((int)player->y, (int)enemy->y);

    /* X axis - close enough? stop horizontal movement */
    if (dx > ENGAGE_RANGE_X)
        enemy->delta_x = 1;
    else if (dx < -ENGAGE_RANGE_X)
        enemy->delta_x = -1;
    else
        enemy->delta_x = 0;

    /* Y axis - close enough? stop vertical movement */
    if (dy > ENGAGE_RANGE_Y)
        enemy->delta_y = 1;
    else if (dy < -ENGAGE_RANGE_Y)
        enemy->delta_y = -1;
    else
        enemy->delta_y = 0;
}
/* Function purpose: Moves the player according to the velocity
 * Input: The player object
 * Output: None, moves the player object position on the screen
 * Assumptions: The player has an appropriate velocity */
void update_player_position(Player *player)
{
    /* Horizontal: guard underflow before moving, clamp max after */
    if (player->delta_x != 0) {
        if (player->delta_x < 0 &&
            player->x < (unsigned int)(-player->delta_x))
        {
            player->x = 0;  /* would underflow — clamp to left edge */
        } else {
            move_player_horizontal(player);
            if (player->x > MAX_X) player->x = MAX_X;
        }
    }

    /* Vertical: same pattern */
    if (player->delta_y != 0) {
        if (player->delta_y < 0 &&
            player->y < (unsigned int)(-player->delta_y))
        {
            player->y = 0;  /* would underflow — clamp to top edge */
        } else {
            move_player_vertical(player);
            if (player->y > MAX_Y) player->y = MAX_Y;
        }
    }
}


/* Function purpose: Moves the enemy according to the velocity
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity */
void update_enemy_position(Enemy *enemy, const Player *player) {
    
    update_enemy_velocity(enemy, player);

    if (enemy->delta_x != 0) {
        move_enemy_horizontal(enemy);
    }
    
    if (enemy->delta_y != 0) {
        move_enemy_vertical(enemy);
    }
}


/* Function purpose: Moves the boss according to the velocity
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity */
void update_boss_position(Boss *boss, const Player *player) {
    /* TODO: Calculate direction to player */

    if (boss->delta_x != 0) {
        move_boss_horizontal(boss);
    }
    
    if (boss->delta_y != 0) {
        move_boss_vertical(boss);
    }
}

/* Function purpose: updates the player cooldowns together
 * Input: The player object
 * Output: None, just updates the cooldowns
 * Assumptions: The other cooldown functions are working properly */
void update_attack_cooldown(Player *player) {
    if (player->attack_cooldown > 0) {
        player->attack_cooldown--;
        
        /* When cooldown expires, reset attacking state */
        if (player->attack_cooldown == 0) {
            player->is_attacking = FALSE;
        }
    }
}

/* Function purpose: Updates the player item cooldown
 * Input: The player object
 * Output: None, decrements the cooldown
 * Assumptions: The function is called at the appropriate timings */
void update_item_cooldown(Player *player) {
    if (player->item_cooldown > 0) { 
        player->item_cooldown--;
    }
}

/* Function purpose: updates the player cooldowns together
 * Input: The player object
 * Output: None, just updates the cooldowns
 * Assumptions: The other cooldown functions are working properly */
void update_player_cooldowns(Player *player) {
    update_attack_cooldown(player);
    update_item_cooldown(player);
}
/*
 * Activates and initializes a wave of enemies for the given stage.
 * Stage 1 spawns 3 enemies, stage 2 spawns 4, stage 3 spawns 5.
 * Enemies are placed off the right edge of the screen.
 * Updates model->enemy_count to include the newly active enemies.
 *
 * Input:  model - the live game model
 *         stage - wave number (1, 2, or 3)
 * Output: modifies model->enemy array and enemy_count
 * Assumptions: MAX_ENEMIES is large enough to hold all waves (14+)
 */
void spawn_enemy(Model *model, int stage)
{
    int i;
    int count;
    int index_offset;

    if (stage == 1) {
        count = 3;
        index_offset = 2;
    } else if (stage == 2) {
        count = 4;
        index_offset = 5;
    } else {
        count = 5;
        index_offset = 9;
    }

    for (i = index_offset; i < index_offset + count; i++)
    {
        model->enemy[i].active = TRUE;
        model->enemy[i].health = 50;
        model->enemy[i].damage = 8;
        model->enemy[i].w = 32;
        model->enemy[i].h = 32;
        model->enemy[i].delta_x = 0;
        model->enemy[i].delta_y = 0;
        model->enemy[i].is_attacking = FALSE;

        /* Stagger spawn positions so they don't stack on top of each other */
        model->enemy[i].x = 620;
        model->enemy[i].y = 168 + ((i - index_offset) * 40); /*Arbitrary for now but staggered*/
    }

    model->enemy_count = index_offset + count;
}



