#include "player.h"

void move_player_vertical(Player *player){
    player->y += player->delta_y;
}

void move_player_horizontal(Player *player){
    player->x += player->delta_x;
}

bool update_health(Player *player, int x){
    bool is_dead = false;
    player->health += x;
    if (player->health <= 0){
        is_dead = true;
    }
    return is_dead;
}
void update_damage(Player *player, int x){
    player->damage += x;
}

int light_attack(Player *player){
    return player->damage;
}
