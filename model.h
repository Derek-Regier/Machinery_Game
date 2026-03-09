/*
 * Game world model definition
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef MODEL_H
#define MODEL_H

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "item.h"
#include "types.h"

typedef struct {
    Player player;
    Enemy enemy[1]; /*Placeholder, actual enemies will depend on gameplay*/
    Boss boss;
    Item item[1]; /*Number of items will also depend on gameplay*/
    bool quit;
} Model;

/*
 *  Function Purpose: Fills *model with the game's initial state:
 *
 * Input: The model
 * Output: None, modifies current game state
 * Assumptions: All valid declarations
 */
void init_model(Model *model);

#endif



