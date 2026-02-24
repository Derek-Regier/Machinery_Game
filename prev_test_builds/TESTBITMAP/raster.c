/*
 * raster.c - Raster graphics library for the Atari ST (C layer).
 *
 * Screen: 640 x 400, monochrome, 80 bytes per row.
 * Pixel (row, col): byte = base[row*80 + col/8], bit = 7-(col%8).
 *
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include "raster.h"

#define SCREEN_WIDTH      640
#define SCREEN_HEIGHT     400
#define BYTES_PER_ROW     80   /* 640 / 8 */
#define WORDS_PER_ROW     40   /* 640 / 16 */
#define LONGS_PER_ROW     20   /* 640 / 32 */

