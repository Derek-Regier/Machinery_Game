/*
 * Item function implementation
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "item.h"

/* Function purpose: get the items value
 * Input: The item object
 * Output: An int of the items value
 * Assumptions: None */
int get_value(Item *item){
    return item->value;

}
