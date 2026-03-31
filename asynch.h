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
#include "effects.h"

/* Asynchronous event handlers */

/* Function purpose: Initiates a light attack calling the player behaviour function 
 * Input: The player object and the cooldown time
 * Output: None, included as part of the light attack sequence 
 * Assumptions: Proper values are passed from the game logic */
void on_light_attack(Player *player, int cooldown);

/* Function purpose: Starts the movement process by setting the appropriate velocity for the given keypress, dictates player facing direction
 * Input: The player object and the input key
 * Output: None, included as part of the movement sequence
 * Assumptions: Proper values are passed from the game logic */
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