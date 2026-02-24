#ifndef RASTER_H
#define RASTER_H
#include "types.h"

/*
 * raster.h - Low-level raster graphics library for the Atari ST.
 *
 * All functions write directly to the frame buffer pointed to by base.
 * No OS or stdlib calls are used. Screen resolution is 640 x 400.
 *
 * For plot_bitmap_8/16/32 and plot_character, the caller must supply
 * the bitmap data. For plot_character, call linea0() once before use
 * and pass the font table pointer returned by (UINT8 *)V_FNT_AD.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

void clear_screen(UINT32 *base);

void clear_region(UINT32 *base, UINT16 row, UINT16 col,
                  UINT16 length, UINT16 width);

void plot_pixel(UINT8 *base, UINT16 row, UINT16 col);

void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col,
                          UINT16 length);

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col,
                        UINT16 length);

void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col,
               UINT16 end_row, UINT16 end_col);

void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col,
                    UINT16 length, UINT16 width);

void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side);

/*
 * direction: 0=top-left anchor, 1=top-right anchor,
 *            2=bottom-left anchor, 3=bottom-right anchor
 */
void plot_triangle(UINT32 *base, UINT16 row, UINT16 col,
                   UINT16 base_len, UINT16 height, UINT8 direction);

/*
 * bitmap: pointer to row data; each entry is one 8-bit row of the sprite.
 * height: number of rows in the bitmap.
 */
void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col,
                   const UINT8 *bitmap, UINT16 height);

void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col,
                    const UINT16 *bitmap, UINT16 height);

void plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col,
                    const UINT32 *bitmap, UINT16 height);

/*
 * font_table: pointer to a 256-glyph 8x8 font table.
 *             Row N of char C is at font_table[C * 8 + N].
 */
void plot_character(UINT8 *base, UINT16 row, UINT16 col,
                    char ch, const UINT8 *font_table);

void plot_string(UINT8 *base, UINT16 row, UINT16 col,
                 const char *str, const UINT8 *font_table);

#endif /* RASTER_H */
