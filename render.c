#include "render.h"
#include "font.h"
#include "synch.h"
#include "input.h"

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
void render(const Model *model, UINT32 *base){
    int i;

    if (!model->started){

        render_splash(base, font, model->quit);
    }else{
        render_dash_trail(&model->player, base);
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
                render_hit_flash(model->enemy[i].x, model->enemy[i].y,
                                 model->enemy[i].w, model->enemy[i].h,
                                 model->player.facing,
                                 model->enemy[i].hit_flash_timer, base);
            }
        }
    
        render_hit_flash(model->player.x, model->player.y,
                         model->player.w, model->player.h,
                         model->player.hit_facing,
                         model->player.hit_flash_timer, base);
    
        if (model->boss.active)
        {
            render_boss_stomp(&model->boss, base);
            render_hit_flash(model->boss.x, model->boss.y,
                             model->boss.w, model->boss.h,
                             model->player.facing,
                             model->boss.hit_flash_timer, base);
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
void render_player(const Player *player, UINT32 *base){
    UINT32 *bitmap;

    if (prev_drawn && player->x == prev_player.x && 
        player->y == prev_player.y && player->facing == prev_player.facing &&
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
void render_enemy(const Enemy *enemy, Enemy *prev, UINT32 *base){
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
    health to update display. Increments or decrements depending on player health.
 * Input: Healthbar object and framebuffer base
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
 * Assumptions: init_model initializes coordinates of model */
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
 * Assumptions: init_model initializes coordinates of model, boss is 128x128*/
void render_boss(const Boss *boss, UINT32 *base){
    UINT32 (*bitmap)[4];

    if (!boss->active)
    {
        if (prev_drawn && prev_boss.active){
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

    if (prev_drawn && prev_boss.active){
        clear_region(base,
                     (UINT16)prev_boss.y,
                     (UINT16)prev_boss.x,
                     (UINT16)prev_boss.h,
                     (UINT16)prev_boss.w);
                    }

    if (boss->anim_frame == 0){
        if (boss->stomp_frame > STOMP_DURATION / 2)
            bitmap = (boss->facing < 0) ? boss_bitmap_left_stomp
                                        : boss_bitmap_right_stomp;
        else
            bitmap = (boss->facing < 0) ? boss_bitmap_left : boss_bitmap_right;
    }else{
        bitmap = (boss->facing < 0) ? boss_bitmap_left_walk1 : boss_bitmap_right_walk1;
    }
    pbm128(base, (UINT16)boss->y, (UINT16)boss->x, bitmap, 128);

    prev_boss = *boss;
}

/* Function purpose: Displays item count on top left of the screen
* Input: on player pickup and framebuffer base
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
* Input: Player Attack and framebuffer base
* Output: Renders visual slash effect coming from the direction the player is facing
* Assumptions: none */
void render_player_slash(const Player *player, UINT32 *base){
    unsigned int slash_x;
    unsigned int slash_y;
    UINT32 *bitmap;

    if (player->attack_cooldown == 0){
        return;
    }
    slash_x = (player->facing > 0)
              ? player->x + 16
              : (unsigned int)((int)player->x - 24);
    slash_y = player->y + 24;

    bitmap = (player->facing > 0) ? slash_bitmap_right : slash_bitmap_left;

    pbm32(base, (UINT16)slash_y, (UINT16)slash_x, bitmap, 32);
}

/* Function purpose: Render slash bitmap when enemy attacks
* Input: Enemy attack and framebuffer base
* Output: Renders visual slash effect coming from the direction the enemy is facing
* Assumptions: none */
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

/* Function purpose: Draws a 16x16 impact star in the upper area of an entity
 *   when its hit_flash_timer is active. The star is offset left or right based
 *   on attacker_facing so it appears on the side the hit came from rather than
 *   dead-centre where it would be hidden by the sprite.
 * Input: entity top-left x/y, entity w/h, attacker_facing (-1/0/1),
 *        hit_flash_timer, framebuffer base
 * Output: Impact star bitmap drawn near upper edge of entity; nothing if timer is 0
 * Assumptions: hit_flash_timer is decremented by the cooldown update path */
void render_hit_flash(unsigned int ex, unsigned int ey,
                      unsigned int ew, unsigned int eh,
                      int attacker_facing,
                      int hit_flash_timer, UINT32 *base)
{
    unsigned int cx;
    unsigned int cy;

    if (hit_flash_timer <= 0) return;

    /* Place star in the upper quarter of the entity on the side the hit came
     * from.  attacker_facing 1  = attacker faced right, hit lands on the
     * entity's left side;  -1 = attacker faced left, hit lands on right side;
     * 0 (unknown) = horizontally centred. */
    cy = ey + (eh / 4) - 8;

    if (attacker_facing > 0)
        cx = ex - 4;              /* left side of entity, slight overhang */
    else if (attacker_facing < 0)
        cx = ex + ew - 12;        /* right side of entity */
    else
        cx = ex + (ew / 2) - 8;  /* centred fallback */

    pbm16((UINT16 *)base, (UINT16)cy, (UINT16)cx, hit_star_bitmap, 16);
}

/* Function purpose: Renders three growing shockwave arcs radiating from the
 *   boss during the second half of the stomp (leg coming down).
 *   Arc size and spacing grow with distance from the boss.
 *   sm arc at 8px, md arc at 28px, lg arc at 56px from the boss edge.
 * Input: Boss object and framebuffer base
 * Output: Three arc bitmaps drawn to the facing side; nothing if stomp is
 *   in the leg-raise phase or is not active
 * Assumptions: boss->facing is -1 or 1; stomp_frame ticks down from
 *   STOMP_DURATION set in synch.h */
void render_boss_stomp(const Boss *boss, UINT32 *base)
{
    unsigned int base_x;
    unsigned int wave_y;
    UINT32 *sm;
    UINT32 *md;
    UINT32 *lg;

    /* Only draw shockwaves in the second half (leg coming down) */
    if (boss->stomp_frame <= 0 || boss->stomp_frame > STOMP_DURATION / 2)
        return;

    /* Waves emit from the boss foot toward the facing direction */
    if (boss->facing > 0) {
        base_x = boss->x + boss->w;  /* right edge of boss */
        sm = shockwave_sm_r;
        md = shockwave_md_r;
        lg = shockwave_lg_r;
    } else {
        /* Left-facing: arcs sit to the left; shift leftward by arc width */
        base_x = (unsigned int)((int)boss->x - 32);
        sm = shockwave_sm_l;
        md = shockwave_md_l;
        lg = shockwave_lg_l;
    }

    /* Vertically centre waves on the bottom third of the boss (foot level) */
    wave_y = boss->y + boss->h - 32;

    /* sm: closest, 8 rows tall */
    pbm32(base, (UINT16)wave_y + 12,
          (UINT16)base_x, sm, 8);

    /* md: 20px further out, 16 rows tall */
    pbm32(base, (UINT16)wave_y + 8,
          (UINT16)((boss->facing > 0)
              ? base_x + 20
              : (unsigned int)((int)base_x - 20)),
          md, 16);

    /* lg: 48px further out, 32 rows tall */
    pbm32(base, (UINT16)wave_y,
          (UINT16)((boss->facing > 0)
              ? base_x + 48
              : (unsigned int)((int)base_x - 48)),
          lg, 32);
}

/* Function purpose: Draws speed-lines behind the player for the duration of a dash trail.
 * Lines run from where the dash began to just behind the player's current position.
 * Three lines at varied Y offsets and stepped lengths give a motion-blur feel.
 * Input: Player object and framebuffer base
 * Output: Horizontal lines drawn on framebuffer; nothing drawn when trail_timer <= 0
 * Assumptions: trail_x/trail_y/trail_facing were set at dash initiation */
void render_dash_trail(const Player *player, UINT32 *base){
    int start_x, length, base_y;
    /* gap between trail end and player sprite edge, so lines don't overlap the sprite */
    int gap = 10;

    if (player->trail_timer <= 0) return;

    base_y = (int)player->trail_y;

    if (player->trail_facing > 0) {
        /* Dashed right: trail_x < player->x; lines sit to the left of current pos */
        start_x = (int)player->trail_x;
        length   = (int)player->x - gap - start_x;
    } else {
        /* Dashed left: trail_x > player->x; lines sit to the right of current pos */
        start_x = (int)player->x + (int)player->w + gap;
        length   = (int)player->trail_x - start_x;
    }

    if (length <= 0) return;

    /* Three lines: full length, trimmed 6px, trimmed 14px — staggered Y for depth */
    plot_horizontal_line(base, base_y + 20, start_x,     (UINT16)length);
    if (length > 6)
        plot_horizontal_line(base, base_y + 32, start_x + 6,  (UINT16)(length - 6));
    if (length > 14)
        plot_horizontal_line(base, base_y + 44, start_x + 14, (UINT16)(length - 14));
}


   /* Function purpose: Renders background
 * Input: framebuffer base
 * Output: displays background elements and vertical line
 * Assumptions: none */
void render_background(UINT32 *base){
   /*horizontal line across screen*/
    plot_horizontal_line(base, 220, 0, 640);
    
    /*top most castle tips*/
    plot_horizontal_line(base, 70, 100, 50);
    plot_horizontal_line(base, 70, 200, 50);
    plot_horizontal_line(base, 70, 300, 50);
    plot_horizontal_line(base, 70, 400, 50);
    plot_horizontal_line(base, 70, 500, 50);

    /*bottom most castle tips*/
    plot_horizontal_line(base, 60, 50, 50);
    plot_horizontal_line(base, 60, 150, 50);
    plot_horizontal_line(base, 60, 250, 50);
    plot_horizontal_line(base, 60, 350, 50);
    plot_horizontal_line(base, 60, 450, 50);
    plot_horizontal_line(base, 60, 550, 50);
    
    /*door*/
    plot_vertical_line(base, 120, 270, 100);
        /*door mid point*/
        plot_vertical_line(base, 120, 325, 100);
    plot_horizontal_line(base, 120, 270, 110);
    plot_vertical_line(base, 120, 380, 100);

    /*castle vertical lines*/
    plot_vertical_line(base, 60, 50, 160);
    plot_vertical_line(base, 60, 100, 30);
    plot_vertical_line(base, 60, 150, 30);
    plot_vertical_line(base, 60, 200, 30);
    plot_vertical_line(base, 60, 250, 30);
    plot_vertical_line(base, 60, 300, 30);
    plot_vertical_line(base, 60, 350, 30);
    plot_vertical_line(base, 60, 400, 30);
    plot_vertical_line(base, 60, 450, 30);
    plot_vertical_line(base, 60, 500, 30);
    plot_vertical_line(base, 60, 550, 30);
    plot_vertical_line(base, 60, 600, 160);

    /*left window 1*/
    plot_vertical_line(base, 120, 100, 40);
    plot_vertical_line(base, 120, 150, 40);
    plot_horizontal_line(base, 120, 100, 50);
    plot_horizontal_line(base, 160, 100, 50);
    plot_rectangle(base, 125, 105, 30, 40);
    /*left window 2*/
    plot_vertical_line(base, 120, 200, 40);
    plot_vertical_line(base, 120, 250, 40);
    plot_horizontal_line(base, 120, 200, 50);
    plot_horizontal_line(base, 160, 200, 50);
    plot_rectangle(base, 125, 205, 30, 40);

    /*right window 1*/
    plot_vertical_line(base, 120, 500, 40);
    plot_vertical_line(base, 120, 550, 40);
    plot_horizontal_line(base, 120, 500, 50);
    plot_horizontal_line(base, 160, 500, 50);
    plot_rectangle(base, 125, 505, 30, 40);

    /*right window 2*/
    plot_vertical_line(base, 120, 400, 40);
    plot_vertical_line(base, 120, 450, 40);
    plot_horizontal_line(base, 120, 400, 50);
    plot_horizontal_line(base, 160, 400, 50);
    plot_rectangle(base, 125, 405, 30, 40);

    /*left triangle tip*/
    plot_line(base, 20, 75, 60, 100);
    plot_line(base, 20, 75, 60, 50);


    /*right triangle tip*/
    plot_line(base, 20, 575, 60, 600);
    plot_line(base, 20, 575, 60, 550);

    /*door handles*/

    plot_rectangle(base, 160, 305, 10, 10);
    plot_rectangle(base, 160, 335, 10, 10);


  
}


/* Function purpose: On player death show renders "YOU ARE DEAD"
 * Input: framebuffer base
 * Output: "YOU ARE DEAD"
 * Assumptions: Player health <= 0 */
void render_death_screen(UINT32 *base)
{
    int r, w;
    UINT16 x = (640 - DEAD_W) / 2;  /* 208 */
    UINT16 y = (400 - DEAD_H) / 2;  /* 189 */
    for (r = 0; r < DEAD_H; r++){
        for (w = 0; w < 7; w++){
            pbm32(base, y + r, x + (w * 32), &dead_bitmap[r][w], 1);
        }
    }       
}  

/*
 * Function purpose: Renders the post-game end screen (death or win).
 * Input: framebuffer base, font, won flag, hovered (0=menu, 1=quit)
 * Output: end screen drawn to base
 */
void render_end_screen(UINT32 *base, const UINT8 *font, bool won, int hovered)
{
    int r, w;
    int mx = get_mouse_x();
    int my = get_mouse_y();

    if (won)
    {
        UINT16 x = (640 - DEAD_W) / 2;
        UINT16 y = 160;
        for (r = 0; r < DEAD_H; r++)
            for (w = 0; w < 7; w++)
                pbm32(base, y + r, x + (w * 32), &you_win_bitmap[r][w], 1);
    }
    else
    {
        UINT16 x = (640 - DEAD_W) / 2;
        UINT16 y = 160;
        for (r = 0; r < DEAD_H; r++)
            for (w = 0; w < 7; w++)
                pbm32(base, y + r, x + (w * 32), &dead_bitmap[r][w], 1);
    }

    /* Main Menu button */
    plot_string(base, END_MENU_Y1 + 8, END_BTN_X1 + 10, "Main Menu", font);
    plot_horizontal_line(base, END_MENU_Y1, END_BTN_X1, 200);
    plot_horizontal_line(base, END_MENU_Y2, END_BTN_X1, 200);
    plot_vertical_line(base, END_MENU_Y1, END_BTN_X1, 25);
    plot_vertical_line(base, END_MENU_Y1, END_BTN_X2, 25);

    /* Quit button */
    plot_string(base, END_QUIT_Y1 + 8, END_BTN_X1 + 10, "Quit", font);
    plot_horizontal_line(base, END_QUIT_Y1, END_BTN_X1, 200);
    plot_horizontal_line(base, END_QUIT_Y2, END_BTN_X1, 200);
    plot_vertical_line(base, END_QUIT_Y1, END_BTN_X1, 25);
    plot_vertical_line(base, END_QUIT_Y1, END_BTN_X2, 25);

    /* Arrow indicator */
    if (hovered == 0)
        pbm8(base, END_MENU_Y1 + 8, END_BTN_X2 + 4, arrow, 8);
    else
        pbm8(base, END_QUIT_Y1 + 8, END_BTN_X2 + 4, arrow, 8);

    /* Crosshair cursor */
    plot_horizontal_line(base, (UINT16)my,
                         (UINT16)(mx > 3 ? mx - 3 : 0), 7);
    plot_vertical_line(base,
                       (UINT16)(my > 3 ? my - 3 : 0),
                       (UINT16)mx, 7);
}

void render_splash(UINT32 *base, const UINT8 *font, bool quit){
    int mx = get_mouse_x();
    int my = get_mouse_y();

    pbm32(base, 100, 100, letter_C, 32);
    pbm32(base, 100, 150, letter_A, 32);
    pbm32(base, 100, 200, letter_S, 32);
    pbm32(base, 100, 250, letter_T, 32);
    pbm32(base, 100, 300, letter_L, 32);
    pbm32(base, 100, 350, letter_E, 32);
    
    pbm32(base, 133, 100, letter_C, 32);
    pbm32(base, 133, 150, letter_R, 32);
    pbm32(base, 133, 200, letter_A, 32);
    pbm32(base, 133, 250, letter_S, 32);
    pbm32(base, 133, 300, letter_H, 32);
    pbm32(base, 133, 350, letter_E, 32);
    pbm32(base, 133, 400, letter_R, 32);
    pbm32(base, 133, 450, letter_S, 32);

    plot_string(base, 199, 125, "1 Player", font);
    plot_horizontal_line(base, 189, 105, 100);
    plot_horizontal_line(base, 214, 105, 100);
    plot_vertical_line(base, 189, 105, 25);
    plot_vertical_line(base, 189, 205, 25);

    plot_string(base, 232, 125, "quit", font);
    plot_horizontal_line(base, 222, 105, 100);
    plot_horizontal_line(base, 247, 105, 100);
    plot_vertical_line(base, 222, 105, 25);
    plot_vertical_line(base, 222, 205, 25);

    plot_string(base, 200, 400, "Instructions:", font);
    plot_string(base, 216, 400, "W,A,S,D for Movement", font);
    plot_string(base, 224, 400, "J=Attack", font);
    plot_string(base, 232, 400, "L=Dash", font);
    plot_string(base, 240, 400, "ESC=Endgame", font);
    plot_rectangle(base, 180, 385, 80, 200);

    if (quit){
        pbm8(base, 232, 210, arrow, 8);
    }else{
        pbm8(base, 199, 210, arrow, 8);
    }

    /* Crosshair cursor: 7-pixel horizontal and vertical arms */
    plot_horizontal_line(base, (UINT16)my,
                         (UINT16)(mx > 3 ? mx - 3 : 0), 7);
    plot_vertical_line(base,
                       (UINT16)(my > 3 ? my - 3 : 0),
                       (UINT16)mx, 7);
}

void render_reset(void){
    prev_drawn = FALSE;
}