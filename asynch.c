/*  
 * Implementation summary: A collection of asynchronous event handlers
 * as implemented from the H file. All keys are hardcoded for now. 
 * All require player input to initiate.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "asynch.h"
#define PLAYER_ATTACK_COOLDOWN 1

/* Function purpose: Initiates a light attack calling the player behaviour function 
 * Input: The player object and the cooldown time
 * Output: None, included as part of the light attack sequence 
 * Assumptions: Proper values are passed from the game logic */
void on_light_attack(Player *player, int cooldown){
    if (cooldown <= 0){
        player_set_attacking(player, TRUE, PLAYER_ATTACK_COOLDOWN);
    }
}

/* Function purpose: Starts the movement process by setting the appropriate velocity for the given keypress 
 * Input: The player object and the input key
 * Output: None, included as part of the movement sequence
 * Assumptions: Proper values are passed from the game logic */
void move_player(Player *player, char key){
    if (key == 'w'){
        player->delta_y = -2; /* Arbitrary start value, animation will determine later*/
    }
    if (key == 's'){
        player->delta_y = 2; 
    }
    if (key == 'a'){
        player->delta_x = -2; 
    }
    if (key == 'd'){
        player->delta_x = 2; 
    }
}

/* Function purpose: use an item
 * Input: The player object and the Item object
 * Output: None
 * Assumptions: Correct objects are passed */
void on_use_item(Player *player, Item *item){
    player->health += item->value;
}


