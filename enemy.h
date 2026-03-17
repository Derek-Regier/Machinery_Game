/*
 * Enemy object defintitions. Struct includes all properties necessary for game logic
 * All functions are used for enemy object manipulation in the game.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    int facing; /*left = -1 and right = 1*/
    unsigned int w, h;
    unsigned int anim_frame;
    unsigned int anim_counter;  
    bool is_attacking; 
    bool active; /* TRUE during enemy attack animation window */
    int attack_cooldown;   /* ticks remaining before enemy can attack again */
    int attack_windup;     /* ticks spent in engage range before striking */
    int y_offset;
} Enemy;


/* Function purpose: Moves the enemy vertically
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity handled from the events modules*/
void move_enemy_vertical(Enemy *enemy);

/* Function purpose: Moves the enemy horizontally
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity handled from the events modules*/
void move_enemy_horizontal(Enemy *enemy);


/* Function purpose: update health for an enemy
 * Input: The enemy object and the health to change
 * Output: Boolean -> Whether the enemy died or not
 * Assumptions: Valid input types */
bool update_enemy_health(Enemy *enemy, int x);


/* Function purpose: update damage for an enemy
 * Input: The enemy object and the damage to change
 * Output: None, updates the enemy object directly
 * Assumptions: Valid input types */
void update_enemy_damage(Enemy *enemy, int x);

/* Function purpose: Get the attack value
 * Input: The enemy object
 * Output: int -> contains the enemy damage output
 * Assumptions: Simple return, assumed function is used correctly */
int enemy_attack(const Enemy *enemy);

/* Function purpose: Determine if an object overlaps with the enemy 
 * Input: The enemy object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the boss considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool enemy_hitbox_overlaps(const Enemy *enemy, int x, int y, int width, int height);

#endif /* ENEMY_H */