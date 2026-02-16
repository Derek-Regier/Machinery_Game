#ifndef ASYNCH_H
#define ASYNCH_H

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "item.h"

/* Asynchronous event handlers */
void on_light_attack(Player *player,int cooldown);
void move_player(Player *player, char key)
void on_use_item(Player *player, Item *item);

#endif /* ASYNCH_H */