#include "boss.h"

void move_boss_vertical(Boss *boss){
    boss->y += boss->delta_y;
}

void move_boss_horizontal(Boss *boss){
    boss->x += boss->delta_x;
}

bool update_boss_health(Boss *boss, int x){
    bool is_dead = false;
    boss->health += x;
    if (boss->health <= 0){
        is_dead = true;
    }
    return is_dead;
}

void update_boss_damage(Boss *boss, int x){
    boss->damage += x;
}

int boss_attack(Boss *boss){
    return boss->damage;
}

bool summon(Boss *boss){
    return boss->health <= (boss->max_health >> 1); // less than 50% health, will need to ensure this triggers only once (probably in main game logic)
}