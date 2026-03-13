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

bool item_hitbox_overlaps(Item *item, int x, int y, int width, int height){
    
    return (item->x == x &&       /* Enemy's right edge is right of player's left edge */
            item->y == y);        /* Enemy's bottom edge is below player's top edge */

}
