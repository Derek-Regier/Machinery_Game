/*
 * Header summary: Bitmap header to reduce redundant extern calls.
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

extern UINT32 building_2[32];

extern UINT32 building_3[32];

extern UINT32 castle[32];

extern UINT32 tree_1[32];

extern UINT32 mountain_1[32];

extern UINT32 dead_bitmap[21][7];

extern UINT32 you_win_bitmap[21][7];

extern UINT16 hit_star_bitmap[16]; /* 16x16 impact spark for hit flash */

extern UINT32 boss_bitmap_right_stomp[128][4]; /* leg raised for stomp */
extern UINT32 boss_bitmap_left_stomp[128][4];

extern UINT32 shockwave_sm_r[8];   /* 8-row  rightward crescent */
extern UINT32 shockwave_sm_l[8];   /* 8-row  leftward  crescent */
extern UINT32 shockwave_md_r[16];  /* 16-row rightward arc      */
extern UINT32 shockwave_md_l[16];  /* 16-row leftward  arc      */
extern UINT32 shockwave_lg_r[32];  /* 32-row rightward arc      */
extern UINT32 shockwave_lg_l[32];  /* 32-row leftward  arc      */

extern UINT32 letter_A[32];
extern UINT32 letter_C[32];
extern UINT32 letter_E[32];
extern UINT32 letter_H[32];
extern UINT32 letter_L[32];
extern UINT32 letter_R[32];
extern UINT32 letter_S[32];
extern UINT32 letter_T[32];

extern UINT8 arrow[8];
  
#endif /* BITMAPS_H */
