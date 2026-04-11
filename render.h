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

 /* Function purpose: Displays item count on top left of the screen
  * Input: on player pickup and framebuffer base
  * Output: Renders visual display of updating health potions
  * Assumptions: none */
void render_item_count(UINT32 *base, const UINT8 *font, const Player *player);

/* Function purpose: Render slash bitmap when player attacks
 * Input: Player Attack and framebuffer base
 * Output: Renders visual slash effect coming from the direction the player is facing
 * Assumptions: none */
void render_player_slash(const Player *player, UINT32 *base);

/* Function purpose: Render slash bitmap when enemy attacks
 * Input: Enemy attack and framebuffer base
 * Output: Renders visual slash effect coming from the direction the enemy is facing  
 * Assumptions: none */
void render_enemy_slash(const Enemy *enemy, UINT32 *base);

/* Function purpose: Draws a 16x16 impact star centred on an entity for
 *   hit_flash_timer ticks after the entity receives damage.
 * Input: entity x/y/w/h, hit_flash_timer, framebuffer base
 * Output: Impact star drawn; nothing if timer is 0
 * Assumptions: hit_flash_timer decremented by cooldown update path */
void render_hit_flash(unsigned int ex, unsigned int ey,
                      unsigned int ew, unsigned int eh,
                      int attacker_facing,
                      int hit_flash_timer, UINT32 *base);

/* Function purpose: Renders three growing shockwave arcs to the facing side
 *   of the boss during the second half of the stomp animation.
 * Input: Boss object and framebuffer base
 * Output: sm/md/lg arcs drawn at increasing distances; nothing if not in
 *   the shockwave phase of the stomp
 * Assumptions: boss->facing is -1 or 1 */
void render_boss_stomp(const Boss *boss, UINT32 *base);

/* Function purpose: Renders background
 * Input: framebuffer base
 * Output: displays background elements and vertical line
 * Assumptions: none */
void render_background(UINT32 *base);

/* Function purpose: Draws speed-lines behind player after a dash
 * Input: Player object and framebuffer base
 * Output: Horizontal trail lines; nothing if trail_timer <= 0 */
void render_dash_trail(const Player *player, UINT32 *base);

/*
 * Button geometry — shared between render.c and crashers.c for hit-testing.
 *
 * Splash screen buttons (left-aligned):
 *   "1 Player": x=105..205, y=189..214
 *   "Quit":     x=105..205, y=222..247
 */
#define SPLASH_BTN_X1   105
#define SPLASH_BTN_X2   205
#define SPLASH_1P_Y1    189
#define SPLASH_1P_Y2    214
#define SPLASH_QUIT_Y1  222
#define SPLASH_QUIT_Y2  247

/*
 * End-screen buttons (centered, 200px wide):
 *   "Main Menu": x=220..420, y=250..275
 *   "Quit":      x=220..420, y=283..308
 */
#define END_BTN_X1   220
#define END_BTN_X2   420
#define END_MENU_Y1  250
#define END_MENU_Y2  275
#define END_QUIT_Y1  283
#define END_QUIT_Y2  308

void render_reset(void);

void render_splash(UINT32 *base, const UINT8 *font, bool quit);

/*
 * Function purpose: Renders the post-game end screen (death or win).
 * Shows the death bitmap or "YOU WIN" text, then two buttons:
 *   hovered == 0 -> arrow points at "Main Menu"
 *   hovered == 1 -> arrow points at "Quit"
 * Input: framebuffer base, font, won flag, hovered button index
 * Output: end screen drawn to base
 */
void render_end_screen(UINT32 *base, const UINT8 *font, bool won, int hovered);

#endif