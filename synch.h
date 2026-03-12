/*
 * Synch events defnitition
 * All synchronous event handlers are defined here for updating movement and cooldowns
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */
#ifndef SYNCH_H
#define SYNCH_H

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "model.h"


/* Function purpose: Moves the player according to the velocity
 * Input: The player object
 * Output: None, moves the player object position on the screen
 * Assumptions: The player has an appropriate velocity */
void update_player_position(Player *player);

/* Function purpose: Moves the enemy according to the velocity
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity */
void update_enemy_position(Enemy *enemy, const Player *player);

/* Function purpose: Moves the boss according to the velocity
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity */
void update_boss_position(Boss *boss, const Player *player);

/* Function purpose: updates the player cooldowns together
 * Input: The player object
 * Output: None, just updates the cooldowns
 * Assumptions: The other cooldown functions are working properly */
void update_player_cooldowns(Player *player);

/* Function purpose: Updates the player attack cooldown
 * Input: The player object
 * Output: None, decrements the cooldown, and resets is attacking to false if cooldown is <=0
 * Assumptions: The function is called at the appropriate timings */
void update_attack_cooldown(Player *player);

/* Function purpose: Updates the player item cooldown
 * Input: The player object
 * Output: None, decrements the cooldown
 * Assumptions: The function is called at the appropriate timings */
void update_item_cooldown(Player *player);

void spawn_enemy(Model *model, int stage);

/*
 * Function purpose: Moves item[stage] to a visible screen position.
 * Called once when the matching wave clears. Stage must be 0-3.
 * Input: The model and the stage that just ended
 * Output: Updates item[stage] position in the model
 * Assumptions: stage is in range; item array has NUM_ITEMS slots
 */
void drop_item(Model *model, int stage);

/* Animation functions TODO */

#endif /* SYNCH_H */
