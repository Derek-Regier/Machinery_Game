/*
 * Player object defintitions. Struct includes all properties necessary for game logic
 * All functions are used for player object manipulation in the game.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */


#ifndef PLAYER_H
#define PLAYER_H
#include "types.h"

typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    bool is_attacking;
    unsigned int item_cooldown;
    unsigned int attack_cooldown;
    unsigned int h,w;
} Player;

/* Function purpose: Moves the player vertically
 * Input: The player object
 * Output: None, moves the player object position on the screen
 * Assumptions: The player has an appropriate velocity handled from the events modules*/
void move_player_vertical(Player *player);

/* Function purpose: Moves the player horizontally
 * Input: The player object
 * Output: None, moves the player object position on the screen
 * Assumptions: The player has an appropriate velocity handled from the events modules*/
void move_player_horizontal(Player *player);


/* Function purpose: update health for the player
 * Input: The player object and the health to change
 * Output: Boolean -> Whether the player died or not
 * Assumptions: Valid input types */
bool update_health(Player *player, int x);

/* Function purpose: update damage for the player
 * Input: The player object and the damage to change
 * Output: None, updates the player object directly
 * Assumptions: Valid input types */
void update_damage(Player *player, int x);


/* Function purpose: Get the player attack value
 * Input: The player object
 * Output: int -> contains the player damage output
 * Assumptions: Simple return, assumed function is used correctly */
int light_attack(const Player *player);

/* Function purpose: Get whether the player is attacking
 * Input: The player object
 * Output: Bool -> returns the attack state
 * Assumptions: Simple return, assumed function is used correctly */
bool player_is_attacking(const Player *player);

/* Function purpose: set the player attacking
 * Input: The player object, the bool whether they are, and the cooldown
 * Output: None
 * Assumptions: correct values are passed */
void player_set_attacking(Player *player, bool attacking, const int cooldown);

/* Function purpose: Determine if an object overlaps with the player 
 * Input: The player object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the player considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool player_hitbox_overlaps(const Player *player, int x, int y, int width, int height);

#endif /* PLAYER_H */


