/*
 * Item function implementation
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */
 
#include "item.h"
#include "types.h"
 
/* Function purpose: get the items value
 * Input: The item object
 * Output: An int of the items value
 * Assumptions: None */
int get_value(Item *item){
    return item->value;
 
}
 
/* Function purpose: Determine if an the player overlaps with the item 
 * Input: The item object, the x position of the other object, the y position, the width, and the height
 * Output: Bool -> if both x and y overlap with the boss considering height and width, return true
 * Assumptions: Valid screen positions and model height and width are passed */
bool item_hitbox_overlaps(const Item *item, int x, int y, int width, int height){
    return (item->x < (unsigned int)(x + width)  &&
            item->x + item->w > (unsigned int)x  &&
            item->y < (unsigned int)(y + height) &&
            item->y + item->h > (unsigned int)y);
}
