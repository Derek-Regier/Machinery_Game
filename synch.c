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
#define SPAWN_DELAY 70      /* ticks between each staggered enemy release (~1 s) */
/* Boss is 128x128; stop when visually adjacent to the 32x64 player */
#define BOSS_ENGAGE_X 96    /* 128 - 32: boss right edge meets player left edge */
#define BOSS_ENGAGE_Y 64    /* 128 - 64: boss bottom edge meets player top edge */

/*
 * Releases one queued enemy every SPAWN_DELAY ticks.
 * Called once per tick from process_sync_events.
 * Does nothing when the queue is empty (spawn_start >= spawn_end).
 *
 * Input:  model - the live game model
 * Output: may activate model->enemy[spawn_start] and advance spawn_start
 */
void update_spawn_queue(Model *model)
{
    if (model->spawn_start >= model->spawn_end) return;

    if (model->spawn_timer > 0) {
        model->spawn_timer--;
        return;
    }

    model->enemy[model->spawn_start].active = TRUE;
    model->spawn_start++;
    model->spawn_timer = SPAWN_DELAY;
}

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
                else if (ex_j > 0)                model->enemy[j].x--;
                if (ey_j >= ey_i && ey_j < MAX_Y) model->enemy[j].y++;
                else if (ey_j > 0)                model->enemy[j].y--;
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
    player->delta_x=0;
    player->delta_y=0;
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

    if (dx > BOSS_ENGAGE_X)
        boss->delta_x = 1;
    else if (dx < -BOSS_ENGAGE_X)
        boss->delta_x = -1;
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
/*
 * Initialises the enemies for the given stage but activates only the first
 * one immediately. The remaining enemies sit inactive in the spawn queue
 * (model->spawn_start .. model->spawn_end-1); update_spawn_queue releases
 * them one per SPAWN_DELAY ticks.
 * Stage 4 (boss summon) always activates both enemies at once.
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
    } else if (stage == 3) {
        count = 5;
        index_offset = 9;
    } else { /* stage 4: boss summon */
        count = 2;
        index_offset = 14;
    }

    for (i = index_offset; i < index_offset + count; i++)
    {
        model->enemy[i].active = FALSE; /* queue, don't activate yet */
        model->enemy[i].health = 50;
        model->enemy[i].damage = 8;
        model->enemy[i].w = 32;
        model->enemy[i].h = 64;
        model->enemy[i].delta_x = 0;
        model->enemy[i].delta_y  = 0;
        model->enemy[i].is_attacking = FALSE;
        model->enemy[i].attack_cooldown = 0;
        model->enemy[i].y_offset = (rand() % 65) - 32;

        if (stage == 4) {
            model->enemy[i].x = (i == index_offset) ? 0 : 608;
            model->enemy[i].y = 200;
        } else {
            model->enemy[i].x = MAX_X + (rand() % 33) - 32;
            model->enemy[i].y = 168 + ((i - index_offset) * 40);
        }
    }

    model->enemy_count = index_offset + count;

    /* Stage 4 activates both summons immediately; others stagger */
    if (stage == 4) {
        model->enemy[index_offset].active = TRUE;
        model->enemy[index_offset + 1].active = TRUE;
        model->spawn_start = index_offset + count; /* queue empty */
        model->spawn_end = index_offset + count;
    } else {
        model->enemy[index_offset].active = TRUE;  /* first one live now */
        model->spawn_start = index_offset + 1;     /* rest queued */
        model->spawn_end   = index_offset + count;
        model->spawn_timer = SPAWN_DELAY;
    }
}

void update_enemy_cooldown(Enemy *enemy)
{
    if (enemy->attack_cooldown > 0)
        enemy->attack_cooldown--;
}

/*
 * Places item[stage] at centre-screen so the player can collect it.
 * Called once per wave when next_level() returns TRUE.
 * All four items begin off-screen in init_model; this makes one visible.
 *
 * Input:  model - the live game model
 *         stage - the wave that just cleared (0-3)
 * Output: moves item[stage] to a visible screen position
 * Assumptions: stage is in range 0-3; item array has NUM_ITEMS slots
 */
void drop_item(Model *model, int stage)
{
    if (stage < 0 || stage >= NUM_ITEMS) return;

    model->item[stage].x = 304; /* roughly centre of 640-wide screen */
    model->item[stage].y = 200;
    model->item[stage].w = 16;
    model->item[stage].h = 16;
    model->item[stage].value = 30;
}