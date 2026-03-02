/*
 * Condition-based (cascaded) event handlers.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef COND_H
#define COND_H

#include "model.h"
#include "synch.h"

/*
 * Function Purpose: Applies damage to the player. 
 * Input: The player and the damage to be taken
 * Output: Returns TRUE if the player died.
 * Assumptions: The damage is already applied negatively, so posisitive values are expected
 */
bool player_take_damage(Player *player, int damage);

/*
 * Function Purpose: Updates the health HUD to reflect the player's current health.
 * Calls into the renderer/raster layer (TODO).
 * Input: The player object
 * Output: None -> Changes the HUD
 * Assumptions: Player's health is appropriate and in bounds.
 */
void update_health_HUD(Player *player);

/*
 * Function Purpose: Checks boss summon threshold and marks boss->summoned.
 * Input: The boss object
 * Output: None -> Updates boss object 
 * Assumptions: Must be called each tick; fires only once due to summoned flag.
 */
void boss_summon(Boss *boss);

/*
 * Function purpose: Checks if the player's attack hitbox overlaps a single enemy.
 * Call once per enemy in the game loop. Applies damage if hit.
 * Input: The player and an enemy object
 * Output: None for now
 * Assumptions: Previous functions work as expected
 */
void player_hits_enemy(Player *player, Enemy *enemy);

/*
 * Function Purpose: Checks if the player's attack hitbox overlaps the boss.
 * Symmetric to player_hits_enemy...
 */
void player_hits_boss(Player *player, Boss *boss);

/*
 * Function Purpose: Checks if a single enemy's hitbox overlaps the player during its
 * attack window. Applies damage and updates HUD if hit.
 * Input: The enemy and player objects
 * Output: Returns TRUE if the player died from this hit.
 */
bool enemy_hits_player(Enemy *enemy, Player *player);

/*
 * Function purpose: Triggered when player health reaches 0. Handles game restart state.
 * TODO for now 
 */
void player_dies(void);

/*
 * Function Purpose: Checks win/lose conditions against the full model.
 * Input: The current game state
 * Output: Returns TRUE if the game should end (either outcome).
 * Assumptions: The current game state is valid
 */
bool level_end(const Model *model);

#endif /* COND_H */

