/*  
 * Implementation summary: A collection of asynchronous event handlers
 * as implemented from the H file. All keys are hardcoded for now. 
 * All require player input to initiate.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "asynch.h"
#include "item.h"
#define PLAYER_ATTACK_COOLDOWN 10
#define ITEM_USE_COOLDOWN 35  /* ~0.5 s at 70 Hz; prevents potion spam */

/* Function purpose: Initiates a light attack calling the player behaviour function 
 * Input: The player object and the cooldown time
 * Output: None, included as part of the light attack sequence 
 * Assumptions: Proper values are passed from the game logic */
void on_light_attack(Player *player, int cooldown){
    if (cooldown <= 0){
        player_set_attacking(player, TRUE, PLAYER_ATTACK_COOLDOWN);
    }
}

/* Function purpose: Starts the movement process by setting the appropriate velocity for the given keypress, dictates player facing direction
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
        player->facing = -1;
    }
    if (key == 'd'){
        player->delta_x = 2; 
        player->facing = 1;
    }
    if (key == 'l'){
        if (player->facing == 1){
            player->delta_x = 75;
        }else{
            player->delta_x = -75;
        }
    }
}

/* Function purpose: Consume one potion from the player's inventory.
 * Heals the player by POTION_HEAL_VALUE, capped at PLAYER_MAX_HEALTH.
 * Does nothing if the player has no potions or item_cooldown is active.
 * Input: The player object
 * Output: None, modifies player->health, potions, item_cooldown
 * Assumptions: Called from async key handler only */
void consume_potion(Player *player){
    if (player->potions <= 0)        return;
    if (player->item_cooldown > 0)   return;

    player->health += POTION_HEAL_VALUE;
    if (player->health > PLAYER_MAX_HEALTH)
        player->health = PLAYER_MAX_HEALTH;

    player->potions--;
    player->item_cooldown = ITEM_USE_COOLDOWN;
}
