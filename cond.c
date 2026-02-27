/*
 * cond.c - Condition-based (cascaded) event handler implementations.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "cond.h"

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
 * has not yet summoned. Sets boss->summoned on first trigger so this
 * fires exactly once per boss encounter.
 */
void boss_summon(Boss *boss)
{
    if (summon(boss) && !boss->summoned) {
        boss->summoned = TRUE;
        /* TODO: spawn enemies from screen edges */
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
    if (!player_is_attacking(player))  return;
    if (enemy->health <= 0)            return;

    if (player_hitbox_overlaps(player,
                               enemy->x, enemy->y,
                               enemy->w, enemy->h))
    {
        int damage = light_attack(player);
        update_enemy_health(enemy, -damage);
        /* TODO: play hit sound, trigger hit animation */
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
 * Returns TRUE if the game has reached a terminal state:
 * The caller should stop the game loop and handle the outcome.
 */
bool level_end(const Model *model)
{
    if (model->boss.health <= 0)    return TRUE;  /* victory   */
    if (model->player.health <= 0)  return TRUE;  /* defeat    */
    return FALSE;
}
