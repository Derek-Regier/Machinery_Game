/*
 * Synch events defnitition
 * All synchronous event handlers are defined here for updating movement and cooldowns
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */
#ifndef SYNCH_H
#define SYNCH_H
#define ENEMY_ATTACK_COOLDOWN 60
#define ENEMY_WINDUP 35          /* ticks spent in range before striking (~0.5 s at 70 Hz) */
#define BOSS_ATTACK_COOLDOWN  90  /* ticks between boss attacks (~1.3 s at 70 Hz) */
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "model.h"

void separate_enemies(Model *model);
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

void update_enemy_cooldown(Enemy *enemy);

/*
 * Function purpose: Sets boss delta_x/delta_y to move toward the player,
 * stopping on each axis once inside engage range. Sets is_attacking when
 * both deltas are zero and attack_cooldown is clear.
 * Input: The boss and player objects
 * Output: Modifies boss->delta_x, delta_y, is_attacking
 * Assumptions: Boss is active; called from update_boss_position
 */
void update_boss_velocity(Boss *boss, const Player *player);

/*
 * Function purpose: Decrements boss attack_cooldown each tick.
 * Input: The boss object
 * Output: None, decrements attack_cooldown toward zero
 * Assumptions: Called once per tick from process_sync_events
 */
void update_boss_cooldown(Boss *boss);

#endif /* SYNCH_H */