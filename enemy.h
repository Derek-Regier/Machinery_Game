#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>

typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    unsigned int w, h;
} Enemy;

void move_enemy_vertical(Enemy *enemy);

void move_enemy_horizontal(Enemy *enemy);

bool update_enemy_health(Enemy *enemy, int x);

void update_enemy_damage(Enemy *enemy, int x);

int enemy_attack(Enemy *enemy);

bool enemy_hitbox_overlaps(const Enemy *enemy, int x, int y, int width, int height);

#endif /* ENEMY_H */