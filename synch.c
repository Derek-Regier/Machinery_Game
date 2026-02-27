#include "synch.h"
#define MAX_X 608 /* 640 pixels - 32 pixels for player */ 
#define MAX_Y 368 /*400 - 32*/

void update_player_position(Player *player)
{
    /* Horizontal: guard underflow before moving, clamp max after */
    if (player->delta_x != 0) {
        if (player->delta_x < 0 &&
            player->x < (unsigned int)(-player->delta_x))
        {
            player->x = 0;  /* would underflow — clamp to left edge */
        } else {
            move_player_horizontal(player);
            if (player->x > MAX_X) player->x = MAX_X;
        }
    }

    /* Vertical: same pattern */
    if (player->delta_y != 0) {
        if (player->delta_y < 0 &&
            player->y < (unsigned int)(-player->delta_y))
        {
            player->y = 0;  /* would underflow — clamp to top edge */
        } else {
            move_player_vertical(player);
            if (player->y > MAX_Y) player->y = MAX_Y;
        }
    }
}

void update_enemy_position(Enemy *enemy, const Player *player) {
    /* TODO: Calculate direction to player */

    if (enemy->delta_x != 0) {
        move_enemy_horizontal(enemy);
    }
    
    if (enemy->delta_y != 0) {
        move_enemy_vertical(enemy);
    }
}

void update_boss_position(Boss *boss, const Player *player) {
    /* TODO: Calculate direction to player */

    if (boss->delta_x != 0) {
        move_boss_horizontal(boss);
    }
    
    if (boss->delta_y != 0) {
        move_boss_vertical(boss);
    }
}

void update_attack_cooldown(Player *player) {
    if (player->attack_cooldown > 0) {
        player->attack_cooldown--;
        
        /* When cooldown expires, reset attacking state */
        if (player->attack_cooldown == 0) {
            player->is_attacking = FALSE;
        }
    }
}
void update_item_cooldown(Player *player) {
    if (player->item_cooldown > 0) { 
        player->item_cooldown--;
    }
}

/* Convenience wrapper that ticks both cooldowns each game tick. */
void update_player_cooldowns(Player *player) {
    update_attack_cooldown(player);
    update_item_cooldown(player);
}
