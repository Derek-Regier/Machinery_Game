/*
 * Synch events implementation
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */
#include <stdlib.h>
#include "synch.h"
#define MAX_X 608       /* 640 - 32 (player/enemy width) */
#define MAX_Y 336       /* 400 - 64 (player/enemy height); bottom clamp so entity stays on screen */
#define MAX_Y_BOSS 272  /* 400 - 128 (boss height); top-left y so bottom edge stays on screen */
#define MAX_X_BOSS 512  /* 640 - 128 (boss width);  top-left x so right edge stays on screen */
#define MIN_Y_WALK 189  /* top of walkable lane: old 264 - 75 extra pixels = 189 */
#define ENGAGE_RANGE_X 31   /* pixels - touching player position bitmap */
#define ENGAGE_RANGE_Y 16   /* pixels - vertical forgiveness */
#define MIN_ENEMY_SEP 36
/* Boss is 128x128; stop when visually adjacent to the 32x64 player */
#define BOSS_ENGAGE_X 31   
#define BOSS_ENGAGE_Y 64    /* 128 - 64: boss bottom edge meets player top edge */

/* Function purpose: Pushes apart any two active enemies that are too close
 *   to each other, preventing them from stacking into a single column while
 *   converging on the player.
 * Input: The game model (reads and writes enemy x/y positions)
 * Output: None, nudges overlapping enemies apart in-place
 * Assumptions: Called once per tick after all enemy positions have been
 *   updated; MAX_ENEMIES, MIN_ENEMY_SEP, MIN_Y_WALK, MAX_X, MAX_Y are
 *   defined and valid 
 * Citations: https://red3d.com/cwr/papers/1987/SIGGRAPH87.pdf
 *            https://gamedev.net/forums/topic/623012-efficient-algorithm-to-stop-enemies-from-converging/
 * Note: We did some googling on how to handle this type of converging. Was a big issue
 */
void separate_enemies(Model *model) {
    int i, j;
    int x_i, y_i, x_j, y_j;
    int dist_x, dist_y;

    /* O(n^2) pairwise scan — each unique pair (i,j) checked exactly once.
     * j always starts at i+1 so we never compare a pair twice or an
     * enemy against itself. */
    for (i = 0; i < MAX_ENEMIES - 1; i++) {
        if (!model->enemy[i].active) continue;
        x_i = (int)model->enemy[i].x;
        y_i = (int)model->enemy[i].y;

        for (j = i + 1; j < MAX_ENEMIES; j++) {
            if (!model->enemy[j].active) continue;
            x_j = (int)model->enemy[j].x;
            y_j = (int)model->enemy[j].y;

            /* Compute absolute distance on each axis independently.
             * We use axis-aligned separation rather than true Euclidean
             * distance to avoid multiplication and stay C89/integer-only. */
            dist_x = x_j - x_i; if (dist_x < 0) dist_x = -dist_x;
            dist_y = y_j - y_i; if (dist_y < 0) dist_y = -dist_y;

            if (dist_x < MIN_ENEMY_SEP && dist_y < MIN_ENEMY_SEP) {

                /* --- X axis: only j is nudged ---
                 * i keeps its position so the inner loop's cached x_i/y_i
                 * remains valid for the rest of j's iterations this frame.
                 * j is pushed away from i: rightward if it is to i's right
                 * (or directly on top), leftward otherwise. Boundary guards
                 * prevent j from leaving the screen. */
                if (x_j >= x_i && x_j < MAX_X) model->enemy[j].x++;
                else if (x_j > 0)               model->enemy[j].x--;

                /* --- Y axis: both i and j are nudged in opposite directions ---
                 * Pushing only j caused a column-stacking bug where enemies
                 * would bunch at the lane boundary. Spreading both entities
                 * means they actively route around each other rather than
                 * one blocking the other indefinitely.
                 * Each nudge is individually clamped to the walkable lane
                 * (MIN_Y_WALK..MAX_Y) so neither enemy is pushed off the path. */
                if (y_j >= y_i) {
                    /* j is below (or level with) i: push j down, i up */
                    if (y_j < MAX_Y)      model->enemy[j].y++;
                    if (y_i > MIN_Y_WALK) model->enemy[i].y--;
                } else {
                    /* j is above i: push j up, i down */
                    if (y_j > MIN_Y_WALK) model->enemy[j].y--;
                    if (y_i < MAX_Y)      model->enemy[i].y++;
                }
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
 * Sets enemy delta_x and delta_y to move toward a cached target position,
 * refreshing the target from the live player position every MOVE_UPDATE_INTERVAL
 * ticks.  Between refreshes the enemy keeps heading toward the last known
 * position, which smooths out direction changes and cuts per-tick recalculation.
 * Stops on each axis independently once inside engage range.
 *
 * Input:  enemy  - the enemy to update velocity for
 *         player - the player to move toward
 * Output: modifies enemy->delta_x, delta_y, move_timer, target_x, target_y
 */
void update_enemy_velocity(Enemy *enemy, const Player *player)
{
    int dx;
    int dy;

    /* Refresh target on expiry; decrement otherwise */
    if (enemy->move_timer <= 0) {
        enemy->target_x = player->x;
        enemy->target_y = player->y;
        enemy->move_timer = MOVE_UPDATE_INTERVAL;
    } else {
        enemy->move_timer--;
    }

    dx = diff((int)enemy->target_x, (int)enemy->x);
    dy = diff((int)enemy->target_y + enemy->y_offset, (int)enemy->y);

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

    if (enemy->delta_x == 0 && enemy->delta_y == 0 && enemy->attack_cooldown == 0) {
        /* In range and cooled down: charge up before striking */
        enemy->attack_windup++;
        if (enemy->attack_windup >= ENEMY_WINDUP) {
            enemy->is_attacking = TRUE;
            enemy->attack_cooldown = ENEMY_ATTACK_COOLDOWN;
            enemy->attack_windup = 0;
        } else {
            enemy->is_attacking = FALSE;
        }
    } else {
        /* Moving or on cooldown: reset the windup so it telegraphs fresh each time */
        enemy->attack_windup = 0;
        enemy->is_attacking = FALSE;
    }
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
        player->delta_x = 0;
    }

    /* Vertical: clamp to walkable lane (MIN_Y_WALK..MAX_Y).
     * Guard unsigned underflow before moving, clamp both bounds after. */
    if (player->delta_y != 0) {
        if (player->delta_y < 0 &&
            player->y < (unsigned int)(-player->delta_y))
        {
            player->y = MIN_Y_WALK;
        } else {
            move_player_vertical(player);
            if (player->y > MAX_Y) player->y = MAX_Y;
            if (player->y < MIN_Y_WALK) player->y = MIN_Y_WALK;
        }
        player->delta_y = 0;
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

    /* Horizontal: guard unsigned underflow then clamp right edge.
     * Mirrors the same pattern used in update_player_position. */
    if (enemy->delta_x < 0 &&
        enemy->x < (unsigned int)(-enemy->delta_x))
    {
        enemy->x = 0;
    }
    else if (enemy->delta_x != 0)
        move_enemy_horizontal(enemy);

    if (enemy->x > MAX_X) enemy->x = MAX_X;

    if (enemy->delta_y != 0)
        move_enemy_vertical(enemy);

    /* Keep enemy within the walkable lane */
    if (enemy->y > MAX_Y) enemy->y = MAX_Y;
    if (enemy->y < MIN_Y_WALK) enemy->y = MIN_Y_WALK;
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
 * Sets boss delta_x and delta_y to move toward a cached target position,
 * refreshing every MOVE_UPDATE_INTERVAL ticks (same cadence as enemies).
 * Stops on each axis independently once inside engage range.
 * Sets is_attacking when the boss is in range and the cooldown is clear.
 *
 * Input:  boss   - the boss to update
 *         player - the player to move toward
 * Output: modifies boss->delta_x, delta_y, is_attacking, move_timer,
 *         target_x, target_y
 */
void update_boss_velocity(Boss *boss, const Player *player)
{
    int dx;
    int dy;

    /* Refresh target on expiry; decrement otherwise */
    if (boss->move_timer <= 0) {
        boss->target_x = player->x;
        boss->target_y = player->y;
        boss->move_timer = MOVE_UPDATE_INTERVAL;
    } else {
        boss->move_timer--;
    }

    dx = diff((int)boss->target_x, (int)boss->x);
    dy = diff((int)boss->target_y, (int)boss->y);

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

    if (boss->delta_x == 0 && boss->delta_y == 0 && boss->attack_cooldown == 0) {
        boss->is_attacking = TRUE;
        boss->stomp_frame = STOMP_DURATION;
    } else
        boss->is_attacking = FALSE;
}

/* Function purpose: Moves the boss according to the velocity
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity */
void update_boss_position(Boss *boss, const Player *player)
{
    unsigned int prev_x = boss->x;
    unsigned int prev_y = boss->y;

    update_boss_velocity(boss, player);

    /* Horizontal: guard unsigned underflow then clamp so the 128px
     * boss sprite never extends past the right screen edge (640px).
     * MAX_X_BOSS = 640 - 128 = 512.  Mirrors update_player_position. */
    if (boss->delta_x < 0 &&
        boss->x < (unsigned int)(-boss->delta_x))
    {
        boss->x = 0;
    }
    else if (boss->delta_x != 0)
        move_boss_horizontal(boss);

    if (boss->x > MAX_X_BOSS) boss->x = MAX_X_BOSS;

    if (boss->delta_y != 0)
        move_boss_vertical(boss);

    /* Boss is 128px tall; clamp to MAX_Y_BOSS so bottom edge stays on screen */
    if (boss->y > MAX_Y_BOSS) boss->y = MAX_Y_BOSS;

    if (boss->x != prev_x || boss->y != prev_y)
    {
        boss->anim_counter++;
        if (boss->anim_counter >= 6)
        {
            boss->anim_frame = !boss->anim_frame;
            boss->anim_counter = 0;
        }
    }
    else
        boss->anim_frame = 0;
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
    if (player->dash_cooldown > 0)
        player->dash_cooldown--;
    if (player->trail_timer > 0)
        player->trail_timer--;
    if (player->hit_flash_timer > 0)
        player->hit_flash_timer--;
}

/* Function purpose: updates the enemy cooldowns
 * Input: The enemy object
 * Output: None, just updates the cooldowns
 * Assumptions: None */
void update_enemy_cooldown(Enemy *enemy)
{
    if (enemy->attack_cooldown > 0)
        enemy->attack_cooldown--;
    if (enemy->hit_flash_timer > 0)
        enemy->hit_flash_timer--;
}

/* Function purpose: updates the boss cooldowns
 * Input: The boss object
 * Output: None, just updates the cooldowns
 * Assumptions: None */
void update_boss_cooldown(Boss *boss)
{
    if (boss->attack_cooldown > 0)
        boss->attack_cooldown--;
    if (boss->hit_flash_timer > 0)
        boss->hit_flash_timer--;
    if (boss->stomp_frame > 0)
        boss->stomp_frame--;
}