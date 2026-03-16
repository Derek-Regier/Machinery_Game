/*
 * Synch events implementation
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */
#include <stdlib.h>
#include "synch.h"
#define MAX_X 608 /* 640 pixels - 32 pixels for player */ 
#define MAX_Y 368 /*400 - 32*/
#define ENGAGE_RANGE_X 31   /* pixels - touching player position bitmap */
#define ENGAGE_RANGE_Y 16   /* pixels - vertical forgiveness */
#define MIN_ENEMY_SEP 36
/* Boss is 128x128; stop when visually adjacent to the 32x64 player */
#define BOSS_ENGAGE_X 31   
#define BOSS_ENGAGE_Y 64    /* 128 - 64: boss bottom edge meets player top edge */

void separate_enemies(Model *model) {
    int i, j, dxi, dyi, ex_i, ey_i, ex_j, ey_j;
    for (i = 0; i < MAX_ENEMIES - 1; i++) {
        if (!model->enemy[i].active) continue;
        ex_i = (int)model->enemy[i].x;
        ey_i = (int)model->enemy[i].y;
        for (j = i + 1; j < MAX_ENEMIES; j++) {
            if (!model->enemy[j].active) continue;
            ex_j = (int)model->enemy[j].x;
            ey_j = (int)model->enemy[j].y;
            dxi = ex_j - ex_i; if (dxi < 0) dxi = -dxi;
            dyi = ey_j - ey_i; if (dyi < 0) dyi = -dyi;
            if (dxi < MIN_ENEMY_SEP && dyi < MIN_ENEMY_SEP) {
                if (ex_j >= ex_i && ex_j < MAX_X) model->enemy[j].x++;
                else if (ex_j > 0) model->enemy[j].x--;
                if (ey_j >= ey_i && ey_j < MAX_Y) model->enemy[j].y++;
                else if (ey_j > 0) model->enemy[j].y--;
            }
        }
    }
}
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
    int dy = diff((int)player->y + enemy->y_offset, (int)enemy->y);

    /* X axis - close enough? stop horizontal movement */
    if (dx > ENGAGE_RANGE_X){
        enemy->delta_x = 1;
        enemy->facing = 1;
    }else if (dx < -ENGAGE_RANGE_X){
        enemy->delta_x = -1;
        enemy->facing = -1;
    }else
        enemy->delta_x = 0;

    /* Y axis - close enough? stop vertical movement */
    if (dy > ENGAGE_RANGE_Y)
        enemy->delta_y = 1;
    else if (dy < -ENGAGE_RANGE_Y)
        enemy->delta_y = -1;
    else
        enemy->delta_y = 0;
    if (enemy->delta_x == 0 && enemy->delta_y == 0 && enemy->attack_cooldown == 0)
        enemy->is_attacking = TRUE;
    else
        enemy->is_attacking = FALSE;
}
   
/* Function purpose: Moves the player according to the velocity
 * Input: The player object
 * Output: None, moves the player object position on the screen
 * Assumptions: The player has an appropriate velocity */
void update_player_position(Player *player)
{
    unsigned int prev_x = player->x;
    unsigned int prev_y = player->y;

    /* Horizontal: guard underflow before moving, clamp max after */
    if (player->delta_x != 0) {
        if (player->delta_x < 0 &&
            player->x < (unsigned int)(-player->delta_x))
        {
            player->x = 0;
        } else {
            move_player_horizontal(player);
            if (player->x > MAX_X) player->x = MAX_X;
        }
        player->facing = (player->delta_x < 0) ? -1 : 1;
    }

    /* Vertical: same pattern */
    if (player->delta_y != 0) {
        if (player->delta_y < 0 &&
            player->y < (unsigned int)(-player->delta_y))
        {
            player->y = 0;
        } else {
            move_player_vertical(player);
            if (player->y > MAX_Y) player->y = MAX_Y;
        }
    }
    /* Animation — flip frame every 4 pixels of movement */
    if (player->x != prev_x || player->y != prev_y)
    {
        player->anim_counter++;
        if (player->anim_counter >= 4)
        {
            player->anim_frame = !player->anim_frame;
            player->anim_counter = 0;
        }
    }
    else
        player->anim_frame = 0;
    /* note: do NOT reset anim_counter to 0 in the else branch */

    player->delta_x = 0;
    player->delta_y = 0;
}




/* Function purpose: Moves the enemy according to the velocity
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity */
void update_enemy_position(Enemy *enemy, const Player *player)
{
    unsigned int prev_x = enemy->x;
    unsigned int prev_y = enemy->y;

    update_enemy_velocity(enemy, player);

    if (enemy->delta_x != 0)
        move_enemy_horizontal(enemy);

    if (enemy->delta_y != 0)
        move_enemy_vertical(enemy);

    if (enemy->x != prev_x || enemy->y != prev_y)
    {
        enemy->anim_counter++;
        if (enemy->anim_counter >= 4)
        {
            enemy->anim_frame = !enemy->anim_frame;
            enemy->anim_counter = 0;
        }
    }
    else
    {
        enemy->anim_frame = 0;
    }
}

/*
 * Sets boss delta_x and delta_y to move toward the player,
 * stopping on each axis independently once inside engage range.
 * Sets is_attacking when the boss is in range and the cooldown is clear.
 *
 * Input:  boss   - the boss to update
 *         player - the player to move toward
 * Output: modifies boss->delta_x, delta_y, and is_attacking
 */
void update_boss_velocity(Boss *boss, const Player *player)
{
    int dx = diff((int)player->x, (int)boss->x);
    int dy = diff((int)player->y, (int)boss->y);

    if (dx > BOSS_ENGAGE_X){
        boss->delta_x = 1;
        boss->facing = 1;
    }else if (dx < -BOSS_ENGAGE_X){
        boss->delta_x = -1;
        boss->facing = -1;
    }
    else
        boss->delta_x = 0;

    if (dy > BOSS_ENGAGE_Y)
        boss->delta_y = 1;
    else if (dy < -BOSS_ENGAGE_Y)
        boss->delta_y = -1;
    else
        boss->delta_y = 0;

    if (boss->delta_x == 0 && boss->delta_y == 0 && boss->attack_cooldown == 0)
        boss->is_attacking = TRUE;
    else
        boss->is_attacking = FALSE;
}

/* Function purpose: Moves the boss according to the velocity
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity */
void update_boss_position(Boss *boss, const Player *player) {
    update_boss_velocity(boss, player);

    if (boss->delta_x != 0) {
        move_boss_horizontal(boss);
    }
    
    if (boss->delta_y != 0) {
        move_boss_vertical(boss);
    }
}

void update_boss_cooldown(Boss *boss)
{
    if (boss->attack_cooldown > 0)
        boss->attack_cooldown--;
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

void update_enemy_cooldown(Enemy *enemy)
{
    if (enemy->attack_cooldown > 0)
        enemy->attack_cooldown--;
}