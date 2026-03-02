/*
 * Enemy object behaviour function implementation
 * All functions are used for enemy object manipulation in the game.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "enemy.h"

/* Function purpose: Moves the enemy vertically
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity handled from the events modules*/
void move_enemy_vertical(Enemy *enemy){
    enemy->y += enemy->delta_y;
}

/* Function purpose: Moves the enemy horizontally
 * Input: The enemy object
 * Output: None, moves the enemy object position on the screen
 * Assumptions: The enemy has an appropriate velocity handled from the events modules*/
void move_enemy_horizontal(Enemy *enemy){
    enemy->x += enemy->delta_x;
}


/* Function purpose: update health for an enemy
 * Input: The enemy object and the health to change
 * Output: Boolean -> Whether the enemy died or not
 * Assumptions: Valid input types */
bool update_enemy_health(Enemy *enemy, int x){
    bool is_dead = FALSE;
    enemy->health += x;
    if (enemy->health <= 0){
        is_dead = TRUE;
    }
    return is_dead;
}

/* Function purpose: update damage for an enemy
 * Input: The enemy object and the damage to change
 * Output: None, updates the enemy object directly
 * Assumptions: Valid input types */
void update_enemy_damage(Enemy *enemy, int x){
    enemy->damage += x;
}


/* Function purpose: Get the attack value
 * Input: The enemy object
 * Output: int -> contains the enemy damage output
 * Assumptions: Simple return, assumed function is used correctly */
int enemy_attack(Enemy *enemy){
    return enemy->damage;
}

/* Function purpose: Determine if an object overlaps with the enemy 
 * Input: The enemy object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the boss considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool enemy_hitbox_overlaps(const Enemy *enemy, int x, int y, int width, int height){  
    return (enemy->x < x + width &&           /* Enemy's left edge is left of player's right edge */
            enemy->x + enemy->w > x &&       /* Enemy's right edge is right of player's left edge */
            enemy->y < y + height &&          /* Enemy's top edge is above player's bottom edge */
            enemy->y + enemy->h > y);        /* Enemy's bottom edge is below player's top edge */

}
