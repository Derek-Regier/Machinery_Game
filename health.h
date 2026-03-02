/*
 * Healthbar object defintitions. Struct is there for position, value, and size only
 * One function for updating value
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Healthbar;

/* Function purpose: Update the healthbar object
 * Input: The healthbar and the value to update
 * Output: An int of the current helath value to be updated on screen
 * Assumptions: correct values are passed */
int update_value(Healthbar *healthbar, int value);

#endif /* HEALTH_H */

