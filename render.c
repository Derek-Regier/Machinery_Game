#include "render.h"

/*
 * Copy each structure as statics for saving coordinates of player, enemy,
 * item, boss, healthbar, and utilize a flag to declare a need for a
 * redraw if the coordinates of the previous structure does not match the new
 * structure. Where prev_drawn = FALSE causes redraw
 */
static Player    prev_player;
static Enemy     prev_enemy0;
static Item      prev_item;
static Boss      prev_boss;
static Healthbar prev_healthbar;
static bool prev_drawn = FALSE;




/* Function purpose: Display game snapshot
 * Input: Game objects (bitmaps and models)
 * Output: Game objects displayed to screen
 * Assumptions: init_model initializes */
void render(const Model *model, UINT32 *base)
{
    int i;
    if(model->player.health <= 0){
      clear_screen(base);
      /*plot_string(base, 200, 200, "You are dead!", font);*/
      return;
    }
    render_player    (&model->player,base);
    for (i = 0; i < MAX_ENEMIES; i ++){
        render_enemy     (&model->enemy[i], &prev_enemy0, base);
    }
    render_healthbar (&model->healthbar, base);
    render_item      (&model->item[0], base);
    render_boss      (&model->boss, base);
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
  

    if (prev_drawn && player->x == prev_player.x && player->y == prev_player.y){
      return;
    }


    if (prev_drawn){
        clear_region(base, prev_player.y, prev_player.x, prev_player.h, prev_player.w);
    }

    pbm32(base, player->y, player->x, player_bitmap, player->h);

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
    if (enemy->active){
        if (prev_drawn &&
            enemy->x == prev->x &&
            enemy->y == prev->y)
            return;
        
        if (prev_drawn)
            clear_region(base, prev->y, prev->x,prev->h, prev->w);
        
        pbm32(base, enemy->y, enemy->x, enemy_bitmap, enemy->h);
        
        *prev = *enemy;
    }
}

/* Function purpose: Displays rectangle serving as a healthbar using player
    health to update display
 * Input: Healthbar object and base
 * Output: Player health bar displayed to top left of screen, proportionate
    to player health value
 * Assumptions: Damage decrements, Healing increments */
void render_healthbar(const Healthbar *healthbar, UINT32 *base)
{
    if (prev_drawn && healthbar->value == prev_healthbar.value){
        return;
    }
    if (prev_drawn){
        clear_region(base, prev_healthbar.y, prev_healthbar.x, prev_healthbar.h, prev_healthbar.w);
    }

    plot_rectangle(base,healthbar->y, healthbar->x, healthbar->h, (UINT16)healthbar->value);

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
void render_boss(const Boss *boss, UINT32 *base)
{
    int r, w;

    if (!boss->active)
    {
        if (prev_drawn && prev_boss.active)
        {
            clear_region(base, prev_boss.y, prev_boss.x,prev_boss.h, prev_boss.w);
            prev_boss.active = FALSE;
        }
        return;
    }

    if (prev_drawn && boss->x == prev_boss.x && boss->y == prev_boss.y && prev_boss.active){
      return;
    }

    if (prev_drawn && prev_boss.active){
        clear_region(base, prev_boss.y, prev_boss.x, prev_boss.h,prev_boss.w);
    }
    for (r = 0; r < 128; r++){
        for (w = 0; w < 4; w++){
            pbm32(base, boss->y + r, boss->x + (w * 32), &boss_bitmap[r][w], 1);
        }
    }
    prev_boss = *boss;
}

void render_reset(void)
{
    prev_drawn = FALSE;
}
