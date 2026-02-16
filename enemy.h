#ifndef ENEMY_H
#define ENEMY_H


typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    unsigned int h,w;
} Enemy;

void move_enemy_vertical(Enemy *enemy);

void move_enemy_horizontal(Enemy *enemy);

bool update_enemy_health(Enemy *enemy, int x);

void update_enemy_damage(Enemy *enemy, int x);

int enemy_attack(Enemy *enemy);

#endif