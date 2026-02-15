#include "enemy.h"

void move_enemy_vertical(Enemy *enemy){
    enemy->y += enemy->delta_y;
}

void move_enemy_horizontal(Enemy *enemy){
    enemy->x += enemy->delta_x;
}

bool update_enemy_health(Enemy *enemy, int x){
    bool is_dead = false;
    enemy->health += x;
    if (ebemy->health <= 0){
        is_dead = true;
    }
    return is_dead;
}

void update_enemy_damage(Enemy *enemy, int x){
    enemy->damage += x;
}

int enemy_attack(Enemy *enemy){
    return enemy->damage;
}