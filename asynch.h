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
void on_light_attack(Player *player,int cooldown);
void move_player(Player *player, char key);
void on_use_item(Player *player, Item *item);

#endif /* ASYNCH_H */

