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

bool item_hitbox_overlaps(const Item *item, int x, int y, int width, int height){
    return (item->x < (unsigned int)(x + width)  &&
            item->x + item->w > (unsigned int)x  &&
            item->y < (unsigned int)(y + height) &&
            item->y + item->h > (unsigned int)y);
}