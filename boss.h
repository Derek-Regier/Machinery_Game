#ifndef BOSS_H
#define BOSS_H

typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    const int max_health;
    unsigned int w, h;
} Boss;

void move_boss_vertical(Boss *boss);

void move_boss_horizontal(Boss *boss);

bool update_boss_health(Boss *boss, int x);

void update_boss_damage(Boss *boss, int x);

int boss_attack(Boss *boss);

bool summon(Boss *boss);


#endif