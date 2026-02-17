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

bool boss_hitbox_overlaps(const Boss *boss, int x, int y, int width, int height){
    return (boss->x < x + width &&           /* Boss's left edge is left of player's right edge */
        boss->x + boss->w > x &&       /* Boss's right edge is right of player's left edge */
        boss->y < y + height &&          /* Boss's top edge is above player's bottom edge */
        boss->y + boss->h > y);        /* Boss's bottom edge is below player's top edge */
}

