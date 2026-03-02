/*
* Healthbar object defintitions. Struct is there for position, value, and size only
 * One function for retrieving value
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef ITEM_H
#define ITEM_H

typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Item;

/* Function purpose: get the items value
 * Input: The item object
 * Output: An int of the items value
 * Assumptions: None */
int get_value(Item *item);

#endif /* ITEM_H */


