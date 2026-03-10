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
#include "health.h"

#define MAX_ENEMIES 10

typedef struct {
    Player player;
    Healthbar healthbar;
    Enemy enemy[MAX_ENEMIES];  /* define MAX_ENEMIES as a constant */
    int enemy_count; 
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
void reset_movement(Model *model);

#endif



