/*
 * Condition-based (cascaded) event handler implementations.
 * Note: For simplicity and further explanation, the header file function docuementation is NOT repeated here
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "cond.h"

void grab_item(Player *player, Item *item){
    
    if (item_hitbox_overlaps(item, player->x, player->y, player->w, player->h)){
        
        on_use_item(player, item);
        
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
        update_health_HUD(player);
        return died;
    }
    return FALSE;
}

/*
 * Called when the player's health reaches zero.
 * Will trigger game-state change to restart. TODO: game state.
 */
void player_dies(void)
{
    /* TODO: transition game state to restart / main menu */
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
