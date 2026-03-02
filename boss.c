/*
 * Boss object behaviour function implementation
 * All functions are used for Boss ibject manipulation in the game.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "boss.h"

/* Function purpose: Moves the boss vertically
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity handled from the events modules*/
void move_boss_vertical(Boss *boss){
    boss->y += boss->delta_y;
}

/* Function purpose: Moves the boss horizontally
 * Input: The boss object
 * Output: None, moves the boss object position on the screen
 * Assumptions: The boss has an appropriate velocity handled from the events modules*/
void move_boss_horizontal(Boss *boss){
    boss->x += boss->delta_x;
}

/* Function purpose: update health for the boss
 * Input: The boss object and the health to change
 * Output: Boolean: Whether the boss died or not
 * Assumptions: Valid input types */
bool update_boss_health(Boss *boss, int x){
    bool is_dead = FALSE;
    boss->health += x;
    if (boss->health <= 0){
        is_dead = TRUE;
    }
    return is_dead;
}

/* Function purpose: update damage for the boss
 * Input: The boss object and the damage to change
 * Output: None, updates the boss object directly
 * Assumptions: Valid input types */
void update_boss_damage(Boss *boss, int x){
    boss->damage += x;
}

/* Function purpose: Get the attack value
 * Input: The boss object
 * Output: int -> contains the boss damage output
 * Assumptions: Simple return, assumed function is used correctly */
int boss_attack(Boss *boss){
    return boss->damage;
}

/* Function purpose: Determine if additional enemies are summoned
 * Input: The boss object
 * Output: Boolean -> return whether the boss met the health threshold for summoning
 * Assumptions: Simple return, assumed function is used correctly */
bool summon(Boss *boss){
    return boss->health <= (boss->max_health >> 1); /* less than 50% health, will need to ensure this triggers only once (probably in main game logic)*/
}

/* Function purpose: Determine if an object overlaps with the boss 
 * Input: The boss object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the boss considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool boss_hitbox_overlaps(const Boss *boss, int x, int y, int width, int height){
    return (boss->x < x + width &&           /* Boss's left edge is left of player's right edge */
        boss->x + boss->w > x &&       /* Boss's right edge is right of player's left edge */
        boss->y < y + height &&          /* Boss's top edge is above player's bottom edge */
        boss->y + boss->h > y);        /* Boss's bottom edge is below player's top edge */
}


