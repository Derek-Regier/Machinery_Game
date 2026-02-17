#ifndef SYNC_H
#define SYNC_H

#include "async.h"

void tick_move_enemy(Enemy *enemy);
void tick_move_boss(Boss *boss);
void tick_move_player(Player *player);
void perform_light_attack(Player *player);
void tick_enemy_attack(Enemy *enemy);
void tick_boss_attack(Boss *boss);
void use_item_cooldown(Player *player, Item *item);

#endif /* SYNC_H */