/*
 * Healthbar function implementation
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "health.h"

/* Function purpose: Update the healthbar object
 * Input: The healthbar and the value to update
 * Output: An int of the current helath value to be updated on screen
 * Assumptions: correct values are passed */
int update_value(Healthbar *healthbar, int value){
    healthbar->value += value;
    return healthbar->value;
}

