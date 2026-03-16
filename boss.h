/*
 * Boss object defintitions. Struct includes all properties necessary for game logic
 * All functions are used for Boss ibject manipulation in the game.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef BOSS_H
#define BOSS_H

#include "types.h"

typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    int facing; /*left = -1 and right = 1*/
    bool summoned;
    int max_health; /* set once at init; treat as read-only afterward */
    bool is_attacking; 
    bool active;     
    unsigned int w, h;
    unsigned int anim_frame;
    unsigned int anim_counter;
    int attack_cooldown; /* ticks remaining before boss can attack again */
} Boss;

/* Function purpose: Moves the boss vertically
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity handled from the events modules*/
void move_boss_vertical(Boss *boss);

/* Function purpose: Moves the boss horizontally
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity handled from the events modules*/
void move_boss_horizontal(Boss *boss);

/* Function purpose: update health for the boss
 * Input: The boss object and the health to change
 * Output: Boolean: Whether the boss died or not
 * Assumptions: Valid input types */
bool update_boss_health(Boss *boss, int x);

/* Function purpose: update damage for the boss
 * Input: The boss object and the damage to change
 * Output: None, updates the boss object directly
 * Assumptions: Valid input types */
void update_boss_damage(Boss *boss, int x);

/* Function purpose: Get the attack value
 * Input: The boss object
 * Output: int -> contains the boss damage output
 * Assumptions: Simple return, assumed function is used correctly */
int boss_attack(Boss *boss);

/* Function purpose: Determine if additional enemies are summoned
 * Input: The boss object
 * Output: Boolean -> return whether the boss met the health threshold for summoning
 * Assumptions: Simple return, assumed function is used correctly */
bool summon(Boss *boss);


/* Function purpose: Determine if an object overlaps with the boss 
 * Input: The boss object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the boss considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool boss_hitbox_overlaps(const Boss *boss, int x, int y, int width, int height);


#endif /* BOSS_H */