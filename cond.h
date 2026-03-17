/*
 * Condition-based (cascaded) event handlers.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef COND_H
#define COND_H
#define SPAWN_DELAY 70  /* ticks between staggered enemy releases */
#include "asynch.h"
#include "types.h"
#include "model.h"
#include "synch.h"

/*
 * Function Purpose: Applies damage to the player. 
 * Input: The player and the damage to be taken
 * Output: Returns TRUE if the player died.
 * Assumptions: The damage is already applied negatively, so positive values are expected
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
 * Spawns two enemies (stage 4) on first trigger; fires exactly once.
 * Input: The boss and the full model (needed to call spawn_enemy)
 * Output: None -> Updates boss and enemy array in model
 * Assumptions: Must be called each tick while boss is active.
 */
void boss_summon(Boss *boss, Model *model);

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
 * Function Purpose: Checks if the boss' hitbox overlaps the player during its
 * attack window. Applies damage and updates HUD if hit.
 * Input: The boss and player objects
 * Output: Returns TRUE if the player died from this hit.
 */
bool boss_hits_player(Boss *boss, Player *player);

/*
 * Function Purpose: return whether the next level is to occur
 * Input: The game model and current stage
 * Output: Returns TRUE if the the next stage is to occur
 * Assumptions: Valid inputs
 */
bool next_level(const Model *model, int stage);

/*
 * Function Purpose: Checks if the player walks over an item and collects it.
 * Increments player->potions and marks item->grabbed on overlap.
 * Does nothing if the item is already grabbed.
 * Input: The player and an item object
 * Output: None -> updates player->potions and item->grabbed
 * Assumptions: item_hitbox_overlaps is a proper AABB check
 */
void grab_item(Player *player, Item *item);
/*
 * Function Purpose: Checks win/lose conditions against the full model.
 * Input: The current game state
 * Output: Returns TRUE if the game should end (either outcome).
 * Assumptions: The current game state is valid
 */
bool level_end(const Model *model);

/*
 * Function purpose: Moves item[stage] to a visible screen position.
 * Called once when the matching wave clears. Stage must be 0-3.
 * Input: The model and the stage that just ended
 * Output: Updates item[stage] position in the model
 * Assumptions: stage is in range; item array has NUM_ITEMS slots
 */
void drop_item(Model *model, int stage);

void spawn_enemy(Model *model, int stage);

/*
 * Function purpose: Releases one queued enemy every SPAWN_DELAY ticks.
 * Call once per tick from process_sync_events.
 * Input: The live game model
 * Output: May activate an enemy and advance spawn_start
 * Assumptions: spawn_start/end/timer were set by spawn_enemy
 */
void update_spawn_queue(Model *model);
#endif /* COND_H */