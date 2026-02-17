/* synch.h */
#ifndef SYNCH_H
#define SYNCH_H

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "model.h"

/* Core Movement Updates */
void update_player_position(Player *player);
void update_enemy_position(Enemy *enemy, const Player *player);
void update_boss_position(Boss *boss, const Player *player);

/* Cooldown Management */
void update_player_cooldowns(Player *player);
void update_attack_cooldown(Player *player);
void update_item_cooldown(Player *player);

/* Animation functions TODO */

#endif /* SYNCH_H */
