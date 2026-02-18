#ifndef COND_H
#define COND_H
#include "synch.h"
#include "model.h"

bool player_take_damage(Player *player, int damage);
void update_health_HUD(Player *player);
void boss_summon(Boss *boss);
void player_hits_enemy(Model *model,int enemy_num);
void enemy_hits_player(Model *model,int enemy_num);
bool level_end(Player *player);
#endif