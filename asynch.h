/*  
 * Header summary: A collection of asynchronous event handlers
 * All require player input to initiate.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef ASYNCH_H
#define ASYNCH_H

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "item.h"

/* Asynchronous event handlers */
void on_light_attack(Player *player, int cooldown);
void move_player(Player *player, char key);

/*
 * Function purpose: Consume one potion from the player's inventory.
 * Heals the player by POTION_HEAL_VALUE, capped at PLAYER_MAX_HEALTH.
 * Does nothing if potions == 0 or item_cooldown > 0.
 * Input: The player object
 * Output: None, modifies player->health, potions, item_cooldown
 * Assumptions: Called from async key handler only
 */
void consume_potion(Player *player);

#endif /* ASYNCH_H */