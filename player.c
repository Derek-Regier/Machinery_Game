#include "player.h"

void move_player_vertical(Player *player){
    player->y += player->delta_y;
}

void move_player_horizontal(Player *player){
    player->x += player->delta_x;
}

bool update_health(Player *player, int x){
    bool is_dead = FALSE;
    player->health += x;
    if (player->health <= 0){
        is_dead = TRUE;
    }
    return is_dead;
}
void update_damage(Player *player, int x){
    player->damage += x;
}

int light_attack(const Player *player){
    return player->damage;
}

bool player_is_attacking(const Player *player){
    return player->is_attacking;
}

void player_set_attacking(Player *player, bool attacking, const int cooldown){
    if (attacking && player->attack_cooldown <= 0){
        player->is_attacking = TRUE;
        player->attack_cooldown = cooldown;
    }
}

bool player_hitbox_overlaps(const Player *player, int x, int y, int width, int height) {

    return (player->x < x + width &&           /* Player's left edge is left of enemy's right edge */
            player->x + player->w > x &&       /* Player's right edge is right of enemy's left edge */
            player->y < y + height &&          /* Player's top edge is above enemy's bottom edge */
            player->y + player->h > y);        /* Player's bottom edge is below enemy's top edge */
}
