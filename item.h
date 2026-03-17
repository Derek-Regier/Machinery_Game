/*
 * Item object defintitions. Struct is there for position, value, and size only
 * One function for retrieving value
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef ITEM_H
#define ITEM_H

#include "types.h"

#define POTION_HEAL_VALUE 30  /* HP restored when a potion is consumed */
#define PLAYER_MAX_HEALTH 100 /* cap for consume_potion heal */

typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
    bool grabbed;
} Item;

/* Function purpose: get the items value
 * Input: The item object
 * Output: An int of the items value
 * Assumptions: None */
int get_value(Item *item);

/* Function purpose: Determine if an the player overlaps with the item 
 * Input: The item object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the boss considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool item_hitbox_overlaps(const Item *item, int x, int y, int width, int height);

#endif /* ITEM_H */
