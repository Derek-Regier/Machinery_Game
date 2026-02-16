#include "async.h"
#define PLAYER_ATTACK_COOLDOWN 1

void on_light_attack(Player *player, int cooldown){
    if (cooldown <= 0){
        player_set_attacking(player, true, PLAYER_ATTACK_COOLDOWN);
    }
}

void move_player(Player *player, char key){
    if (key == 'w'){
        player->delta_y = 1; // Arbitrary start value, animation will determine later
        move_player_vertical(player);
    }
    if (key == 's'){
        player->delta_y = -1; 
        move_player_vertical(player);
    }
    if (key == 'a'){
        player->delta_x = -1; 
        move_player_horizontal(player);
    }
    if (key == 'd'){
        player->delta_x= 1; 
        move_player_horizontal(player);
    }
}

void on_use_item(Player *player, Item *item){
    player->health += item->value;
}