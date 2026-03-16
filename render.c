#include "render.h"
#include "font.h"

/*
 * Copy each structure as statics for saving coordinates of player, enemy,
 * item, boss, healthbar, and utilize a flag to declare a need for a
 * redraw if the coordinates of the previous structure does not match the new
 * structure. Where prev_drawn = FALSE causes redraw
 */
static Player prev_player;
static Enemy prev_enemy[MAX_ENEMIES];
static Item prev_item;
static Boss prev_boss;
static Healthbar prev_healthbar;
static bool prev_drawn = FALSE;
static bool prev_slash = FALSE;
static unsigned int prev_slash_x = 0;
static unsigned int prev_slash_y = 0;

/* Function purpose: Display game snapshot
 * Input: Game objects (bitmaps and models)
 * Output: Game objects displayed to screen
 * Assumptions: init_model initializes */
void render(const Model *model, UINT32 *base)
{
    int i;

    if (model->player.health <= 0)
    {
        clear_screen(base);
        render_death_screen(base);
        return;
    }

    render_player(&model->player, base);

    for (i = 0; i < MAX_ENEMIES; i++){
        render_enemy(&model->enemy[i], &prev_enemy[i], base);
    }
    
    render_healthbar(&model->healthbar, base);
    for (i = 0; i < NUM_ITEMS; i ++){
        render_item (&model->item[i], base);
    }
    render_boss(&model->boss, base);
    render_item_count(base, font, &model->player);
    render_player_slash(&model->player, base);
    render_background(base);    
    
    for (i = 0; i < model->enemy_count; i++){
        if (model->enemy[i].active){
            render_enemy_slash(&model->enemy[i], base);
        }
    }
    prev_drawn = TRUE;
}

/* Function purpose: Displays player bitmap according to player's
    position and compares previous coordinates to new coordinates if player
    needs to be redrawn
 * Input: Player bitmap, models and prev_drawn flag
 * Output: Player object displayed to screen
 * Assumptions: init_model initializes coordinates of model */
void render_player(const Player *player, UINT32 *base)
{
    UINT32 *bitmap;

    if (prev_drawn && player->x == prev_player.x && player->y == prev_player.y && player->facing == prev_player.facing &&
        player->anim_frame == prev_player.anim_frame){
        return;
    }
    if (prev_drawn){
        clear_region(base, prev_player.y, prev_player.x, prev_player.h, prev_player.w);
    }

    if (player->anim_frame == 0){
        bitmap = (player->facing < 0) ? player_bitmap_left : player_bitmap_right;
    }else{
        bitmap = (player->facing < 0) ? player_bitmap_left_walk1 : player_bitmap_right_walk1;
    }

    pbm32(base, player->y, player->x, bitmap, player->h);

    prev_player = *player;
}

/* Function purpose: Displays enemy bitmap according to enemy's position
    and compares previous coordinates to new coordinates if enemy
    needs to be redrawn
 * Input: Enemy bitmap, model, prev pointer for this enemy instance
 * Output: Enemy object displayed to screen
 * Assumptions: init_model initializes coordinates of model */
void render_enemy(const Enemy *enemy, Enemy *prev, UINT32 *base)
{
    UINT32 *bitmap;

    if (!enemy->active){
        if (prev_drawn && prev->active){
            clear_region(base, prev->y, prev->x, prev->h, prev->w);
            prev->active = FALSE;
        }
        return;
    }

    if (prev_drawn &&
        enemy->x == prev->x &&
        enemy->y == prev->y &&
        enemy->facing == prev->facing &&
        enemy->anim_frame == prev->anim_frame){
        return;
        }

    if (prev_drawn){
        clear_region(base, prev->y, prev->x, prev->h, prev->w);
    }
    if (enemy->anim_frame == 0){
        bitmap = (enemy->facing < 0) ? enemy_bitmap_left : enemy_bitmap_right;
    }else{
        bitmap = (enemy->facing < 0) ? enemy_bitmap_left_walk1 : enemy_bitmap_right_walk1;
    }
    pbm32(base, enemy->y, enemy->x, bitmap, enemy->h);

    *prev = *enemy;
}

/* Function purpose: Displays rectangle serving as a healthbar using player
    health to update display
 * Input: Healthbar object and base
 * Output: Player health bar displayed to top left of screen, proportionate
    to player health value
 * Assumptions: Damage decrements, Healing increments */
void render_healthbar(const Healthbar *healthbar, UINT32 *base)
{
    int i;
    plot_rectangle(base,healthbar->y, healthbar->x, healthbar->h, 100);
    if (prev_drawn && healthbar->value == prev_healthbar.value){
        return;
    }
    if (prev_drawn){
        clear_region(base, prev_healthbar.y, prev_healthbar.x, prev_healthbar.h, prev_healthbar.w);
    }
    for(i = 0; i<=10; i++)
    plot_horizontal_line(base, healthbar->y+i, healthbar->x, (UINT16)healthbar->value);
  

    prev_healthbar = *healthbar;
}


/* Function purpose: Displays item bitmap according to item's position
 * Input: Item bitmap and models
 * Output: Item object displayed to screen
 * Assumptions: init_model initializes coordinates of model, enemies drop items */
void render_item(const Item *item, UINT32 *base)
{

    if (!item->grabbed){
        if (prev_drawn && item->x == prev_item.x && item->y == prev_item.y){
            return;
        }
        if (prev_drawn){
            clear_region(base, prev_item.y, prev_item.x, prev_item.h, prev_item.w);
        }
        
        pbm16((UINT16 *)base, item->y, item->x, hp_pot_bitmap, item->h);
        
        prev_item = *item;
    }
}

/* Function purpose: Displays boss bitmap according to boss's position
 * Input: Boss bitmap and model
 * Output: Boss object displayed to screen
 * Assumptions: init_model initializes coordinates of model, boss is also
    128x128 requiring a loop to display full bitmap */
void render_boss(const Boss *boss, UINT32 *base){
    UINT32 (*bitmap)[4];

    if (!boss->active)
    {
        if (prev_drawn && prev_boss.active)
        {
            clear_region(base,
                         (UINT16)prev_boss.y,
                         (UINT16)prev_boss.x,
                         (UINT16)prev_boss.h,
                         (UINT16)prev_boss.w);
            prev_boss.active = FALSE;
        }
        return;
    }

    if (prev_drawn &&
        boss->x == prev_boss.x &&
        boss->y == prev_boss.y &&
        boss->facing == prev_boss.facing &&
        boss->anim_frame == prev_boss.anim_frame &&
        prev_boss.active)
        return;

    if (prev_drawn && prev_boss.active)
        clear_region(base,
                     (UINT16)prev_boss.y,
                     (UINT16)prev_boss.x,
                     (UINT16)prev_boss.h,
                     (UINT16)prev_boss.w);

    if (boss->anim_frame == 0)
        bitmap = (boss->facing < 0) ? boss_bitmap_left : boss_bitmap_right;
    else
        bitmap = (boss->facing < 0) ? boss_bitmap_left_walk1 : boss_bitmap_right_walk1;

    pbm128(base, (UINT16)boss->y, (UINT16)boss->x, bitmap, 128);

    prev_boss = *boss;
}

    /* Function purpose: Displays item count on top left of the screen
     * Input: pickup
     * Output: Renders visual display of updating health potions
     * Assumptions: none */
void render_item_count(UINT32 *base, const UINT8 *font, const Player *player){
    
    int p = (int)player->potions + '0';
    char s = (char)p;
    plot_string(base, 4, 150, "Potions:", font);
    
    plot_character(base, 4, 250, s, font);
    
    p--;
}

    /* Function purpose: Render slash bitmap when player attacks
     * Input: Player attack key
     * Output: Renders visual slash bitmap
     * Assumptions: none */
void render_player_slash(const Player *player, UINT32 *base){
    unsigned int slash_x;
    unsigned int slash_y;
    UINT32 *bitmap;

    if (!player->is_attacking){
        return;
    }
    slash_x = (player->facing > 0)
              ? player->x + 16
              : (unsigned int)((int)player->x - 16);
    slash_y = player->y + 24;

    bitmap = (player->facing > 0) ? slash_bitmap_right : slash_bitmap_left;

    pbm32(base, (UINT16)slash_y, (UINT16)slash_x, bitmap, 32);
}

void render_enemy_slash(const Enemy *enemy, UINT32 *base){
   
    unsigned int slash_x;
    unsigned int slash_y;
    UINT32 *bitmap;

    if (!enemy->is_attacking){
        return;
    }
    slash_x = (enemy->facing > 0)
              ? enemy->x + 16
              : (unsigned int)((int)enemy->x - 16);
    
    slash_y = enemy->y + 24;

    bitmap = (enemy->facing > 0) ? slash_bitmap_right : slash_bitmap_left;

    pbm32(base, (UINT16)slash_y, (UINT16)slash_x, bitmap, 32);
}

void render_background(UINT32 *base){
    pbm32(base, 30, 300, building_1, 32);

}

void render_death_screen(UINT32 *base)
{
    int r, w;
    UINT16 x = (640 - DEAD_W) / 2;  /* 208 */
    UINT16 y = (400 - DEAD_H) / 2;  /* 189 */

    for (r = 0; r < DEAD_H; r++)
        for (w = 0; w < 7; w++)
            pbm32(base, y + r, x + (w * 32), &dead_bitmap[r][w], 1);
}

void render_reset(void){
    prev_drawn = FALSE;
}
