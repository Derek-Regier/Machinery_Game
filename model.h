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

#define MAX_ENEMIES 16  /* 2 tutorial + 3 + 4 + 5 wave + 2 boss summon */
#define NUM_ITEMS   4   /* one item drop per wave (stages 0-3) */

typedef struct {
    Player player;
    Healthbar healthbar;
    Enemy enemy[MAX_ENEMIES];
    int enemy_count;
    Boss boss;
    Item item[NUM_ITEMS];   /* item[stage] is dropped when that wave clears */
    int stage;              /* current wave: 0=tutorial, 1-3=waves, 4=boss */
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
