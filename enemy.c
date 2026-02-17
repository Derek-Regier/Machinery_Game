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
    if (enemy->health <= 0){
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

bool enemy_hitbox_overlaps(const Enemy *enemy, int x, int y, int width, int height){  
    return (enemy->x < x + width &&           /* Enemy's left edge is left of player's right edge */
            enemy->x + enemy->w > x &&       /* Enemy's right edge is right of player's left edge */
            enemy->y < y + height &&          /* Enemy's top edge is above player's bottom edge */
            enemy->y + enemy->h > y);        /* Enemy's bottom edge is below player's top edge */
}