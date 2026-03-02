
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

extern UINT32 player_bitmap[64];

extern UINT32 health_bar_bitmap[12];

extern UINT32 enemy_bitmap[64];

extern UINT32 boss_bitmap[128][4];

extern UINT16 hp_pot_bitmap[16];

#endif /* BITMAPS_H */