/*
 * Condition-based (cascaded) event handler implementations.
 * Note: For simplicity and further explanation, the header file function docuementation is NOT repeated here
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "cond.h"
#include "synch.h"
#include <stdlib.h>
#define MAX_X 608       /* 640 - 32 (player/enemy width) */
#define MAX_Y 336       /* 400 - 64 (player/enemy height) */
#define MIN_Y_WALK 189  /* top of walkable lane */

/*
 * Checks if the player overlaps an item and collects it into inventory.
 * Increments player->potions by 1 and marks item->grabbed so it is not
 * picked up again. Does nothing if the item is already grabbed.
 */
void grab_item(Player *player, Item *item){
    if (item->grabbed) return;

    if (item_hitbox_overlaps(item,
                             (int)player->x, (int)player->y,
                             (int)player->w, (int)player->h))
    {
        player->potions++;
        item->grabbed = TRUE;
    }
}

/*
 * Subtracts damage from player health.
 * Returns TRUE if player health has reached zero (player died).
 */
bool player_take_damage(Player *player, int damage)
{
    player->health -= damage;
    return player->health <= 0;
}

/*
 * Updates health HUD visual to match current player health.
 * Renderer/raster implementation is TODO.
 */
void update_health_HUD(Player *player)
{
    /* TODO: call raster layer to redraw health bar */
}

/*
 * Checks whether the boss has crossed the 50% health threshold and
 * has not yet summoned. On first trigger, activates two enemies from
 * the boss-summon pool (stage 4 in spawn_enemy) and sets boss->summoned
 * so this fires exactly once per encounter.
 */
void boss_summon(Boss *boss, Model *model)
{
    if (summon(boss) && !boss->summoned) {
        boss->summoned = TRUE;
        spawn_enemy(model, 4);
    }
}

/*
 * If the player is currently attacking, checks whether the player
 * hitbox overlaps the given enemy. Applies light attack damage to
 * the enemy on collision. Enemy health going to zero is handled by
 * update_enemy_health returning TRUE - caller may respond to that.
 * Does nothing if enemy is already dead (health <= 0).
 */
void player_hits_enemy(Player *player, Enemy *enemy)
{
    bool died;

    if (!player_is_attacking(player)) return;
    if (!enemy->active)               return;  /* already dead, skip */

    if (player_hitbox_overlaps(player,
                               enemy->x, enemy->y,
                               enemy->w, enemy->h))
    {
        int damage = light_attack(player);
        died = update_enemy_health(enemy, -damage);
        if (died)
            enemy->active = FALSE;
        /* TODO: hit sound, animation */
    }
}

/*
 * Same collision check as player_hits_enemy but against the boss.
 * Boss death is signalled by update_boss_health returning TRUE.
 */
void player_hits_boss(Player *player, Boss *boss)
{
    if (!player_is_attacking(player)) return;
    if (boss->health <= 0)            return;

    if (player_hitbox_overlaps(player,
                               boss->x, boss->y,
                               boss->w, boss->h))
    {
        int damage = light_attack(player);
        update_boss_health(boss, -damage);
        /* TODO: play hit sound, trigger hit animation */
    }
}

/*
 * If the enemy is in its attack window, checks whether its hitbox
 * overlaps the player. Applies damage and updates the HUD on hit.
 * Returns TRUE if the player died from this hit.
 */
bool enemy_hits_player(Enemy *enemy, Player *player)
{
    if (!enemy->is_attacking) return FALSE;

    if (enemy_hitbox_overlaps(enemy,
                              player->x, player->y,
                              player->w, player->h))
    {
        int damage = enemy_attack(enemy);
        bool died  = player_take_damage(player, damage);
        enemy->attack_cooldown = ENEMY_ATTACK_COOLDOWN;
        enemy->is_attacking = FALSE;
        return died;
    }
    return FALSE;
}

bool boss_hits_player(Boss *boss, Player *player)
{
    if (!boss->is_attacking) return FALSE;

    if (boss_hitbox_overlaps(boss,
                              player->x, player->y,
                              player->w, player->h))
    {
        int damage = boss_attack(boss);
        bool died  = player_take_damage(player, damage);
        boss->attack_cooldown = BOSS_ATTACK_COOLDOWN;
        boss->is_attacking = FALSE;
        update_health_HUD(player);
        return died;
    }
    return FALSE;
}
/*
 * Returns TRUE when every enemy in the current wave has been defeated
 * (active == FALSE). Index layout mirrors spawn_enemy:
 *   stage 0 (tutorial) : indices 0-1   (set by init_model)
 *   stage 1            : indices 2-4
 *   stage 2            : indices 5-8
 *   stage 3            : indices 9-13
 * The boss stage (4) is not checked here; level_end() handles that.
 *
 * Input:  model - the live game model
 *         stage - current wave number (0-3)
 * Output: TRUE if all enemies in the wave are inactive
 */
bool next_level(const Model *model, int stage)
{
    int index_offset;
    int count;
    int i;

    /* Spawn queue not empty: more enemies still to arrive this wave */
    if (model->spawn_start < model->spawn_end) return FALSE;

    if (stage == 0) {
        index_offset = 0;
        count        = 2;
    } else if (stage == 1) {
        index_offset = 2;
        count = 3;
    } else if (stage == 2) {
        index_offset = 5;
        count = 4;
    } else {
        index_offset = 9;
        count = 5;
    }

    for (i = index_offset; i < index_offset + count; i++)
    {
        if (model->enemy[i].active) return FALSE;
    }
    return TRUE;
}

/*
 * Returns TRUE if the game has reached a terminal state:
 * The caller should stop the game loop and handle the outcome.
 */
bool level_end(const Model *model)
{
    if (model->boss.health <= 0)    return TRUE;  /* victory   */
    if (model->player.health <= 0)  return TRUE;  /* defeat    */
    return FALSE;
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
    model->item[stage].y = 263; /* centre of walkable lane (189-336) */
    model->item[stage].w = 16;
    model->item[stage].h = 16;
    model->item[stage].value = 30;
    model->item[stage].grabbed = FALSE; /* now live and collectable */
}

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
        model->enemy[i].delta_y = 0;
        model->enemy[i].is_attacking = FALSE;
        model->enemy[i].attack_cooldown = 0;
        model->enemy[i].attack_windup = 0;
        model->enemy[i].y_offset = (rand() % 65) - 32;

        if (stage == 4) {
            /* Boss summon: one from each side, centred in lane */
            model->enemy[i].x = (i == index_offset) ? 0 : 608;
            model->enemy[i].y = 263;
        } else {
            /* Regular wave: spawn from right edge, spread Y across walkable
             * lane (189-336).  Base 209 + slot*32 gives:
             *   slot 0 -> 209, 1 -> 241, 2 -> 273, 3 -> 305, 4 -> 337 (clamped)
             * All within or at the lane boundary. */
            int slot    = i - index_offset;
            int spawn_y = 209 + slot * 32;
            if (spawn_y > MAX_Y) spawn_y = MAX_Y;
            model->enemy[i].x = MAX_X + (rand() % 33) - 32;
            model->enemy[i].y = spawn_y;
        }
    }

    model->enemy_count = index_offset + count;

    /* Stage 4 activates both summons immediately; others stagger */
    if (stage == 4) {
        model->enemy[index_offset].active     = TRUE;
        model->enemy[index_offset + 1].active = TRUE;
        model->spawn_start = index_offset + count; /* queue empty */
        model->spawn_end = index_offset + count;
    } else {
        model->enemy[index_offset].active = TRUE;  /* first one live now */
        model->spawn_start = index_offset + 1;     /* rest queued */
        model->spawn_end = index_offset + count;
        model->spawn_timer = SPAWN_DELAY;
    }
}