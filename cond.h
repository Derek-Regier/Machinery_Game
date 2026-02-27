/*
 * cond.h - Condition-based (cascaded) event handlers.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef COND_H
#define COND_H

#include "model.h"
#include "synch.h"

/*
 * Applies damage to the player. Returns TRUE if the player died.
 */
bool player_take_damage(Player *player, int damage);

/*
 * Updates the health HUD to reflect the player's current health.
 * Calls into the renderer/raster layer (TODO).
 */
void update_health_HUD(Player *player);

/*
 * Checks boss summon threshold and marks boss->summoned.
 * Must be called each tick; fires only once due to summoned flag.
 */
void boss_summon(Boss *boss);

/*
 * Checks if the player's attack hitbox overlaps a single enemy.
 * Call once per enemy in the game loop. Applies damage if hit.
 */
void player_hits_enemy(Player *player, Enemy *enemy);

/*
 * Checks if the player's attack hitbox overlaps the boss.
 * Symmetric to player_hits_enemy.
 */
void player_hits_boss(Player *player, Boss *boss);

/*
 * Checks if a single enemy's hitbox overlaps the player during its
 * attack window. Applies damage and updates HUD if hit.
 * Returns TRUE if the player died from this hit.
 */
bool enemy_hits_player(Enemy *enemy, Player *player);

/*
 * Triggered when player health reaches 0. Handles game restart state.
 */
void player_dies(void);

/*
 * Checks win/lose conditions against the full model.
 * Returns TRUE if the game should end (either outcome).
 */
bool level_end(const Model *model);

#endif /* COND_H */
