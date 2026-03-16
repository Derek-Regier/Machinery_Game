
/*
 * Header summary: Bitmap header to reduce redundant extern calls.
 * - square
 * - rectangle
 * - triangle
 * - player
 * - health_bar
 * - enemy
 * - boss
 * - hp_pot
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */
#ifndef BITMAPS_H
#define BITMAPS_H

#include "types.h"

extern UINT8 square_bitmap[8];

extern UINT16 rect_bitmap[16];

extern UINT32 tri_bitmap[32];

extern UINT32 player_bitmap_right[64];

extern UINT32 player_bitmap_right_walk1[64];

extern UINT32 player_bitmap_left[64];

extern UINT32 player_bitmap_left_walk1[64];

extern UINT32 health_bar_bitmap[12];

extern UINT32 enemy_bitmap_right[64];

extern UINT32 enemy_bitmap_right_walk1[64];

extern UINT32 enemy_bitmap_left[64];

extern UINT32 enemy_bitmap_left_walk1[64];

extern UINT32 boss_bitmap_right[128][4];

extern UINT32 boss_bitmap_right_walk1[128][4];

extern UINT32 boss_bitmap_left[128][4];

extern UINT32 boss_bitmap_left_walk1[128][4];

extern UINT16 hp_pot_bitmap[16];

extern UINT32 slash_bitmap_right[32];  /* 32x32 slash effect */

extern UINT32 slash_bitmap_left[32];  /* 32x32 slash effect */

extern UINT32 building_1[32];
#endif /* BITMAPS_H */