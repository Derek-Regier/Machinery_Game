/*
 * render.h - Renders all game objects
 *
 * Draws player, enemy, boss, item, and healthbar to screen using
 * raster and bitmap modules.
 * 
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include "model.h"
#include "raster.h"
#include "bitmaps.h"
#include "font.h"
#include "asynch.h"
/* Function purpose: Display game snapshot
 * Input: Game objects (bitmaps and models)
 * Output: Game objects displayed to screen
 * Assumptions: init_model initializes*/ 
void render(const Model *model, UINT32 *base);

/* Function purpose: Displays player bitmap according to player's position
 * Input: Player bitmap and models
 * Output: Player object displayed to screen
 * Assumptions: init_model initializes coordinates of model */
void render_player(const Player *player, UINT32 *base);

/* Function purpose: Displays enemy bitmap according to enemy's position
 * Input: Enemy bitmap and model
 * Output: Enemy object displayed to screen
 * Assumptions: init_model initializes coordinates of model*/
void render_enemy(const Enemy *enemy, Enemy *prev, UINT32 *base);


/* Function purpose: Displays boss bitmap according to boss's position
 * Input: Boss bitmap and model 
 * Output: Boss object displayed to screen
 * Assumptions: init_model initializes coordinates of model, boss is also 128x128 requiring a loop to display full bitmap*/
void render_boss(const Boss *boss, UINT32 *base);

/* Function purpose: Displays item bitmap according to item's position
 * Input: Item bitmap and models
 * Output: Item object displayed to screen
 * Assumptions: init_model initializes coordinates of model, enemies drop items*/
void render_item(const Item *item, UINT32 *base);

/* Function purpose: Displays rectangle serving as a health_bar using player health to update display
 * Input: Player health
 * Output: Player health bar displayed to screen. Either being static, incrementing or decrementing. 
 * Assumptions: Damange decrements, Healing increments*/
void render_healthbar(const Healthbar *healthbar, UINT32 *base);

void render_item_count(UINT32 *base, const UINT8 *font, const Player *player);

void render_reset(void);

#endif /* RENDER_H */
