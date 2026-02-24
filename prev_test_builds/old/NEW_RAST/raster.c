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

/* ------------------------------------------------------------------ */
/* clear_screen                                                         */
/* ------------------------------------------------------------------ */

 void clear_screen(UINT32 *base)
{
    UINT32 i;
    for (i = 0; i < 8000; i++)  /* 32000 bytes / 4 = 8000 longs */
        base[i] = 0;
}
/* ------------------------------------------------------------------ */
/* clear_region                                                         */
/* ------------------------------------------------------------------ 

void clear_region(UINT32 *base, UINT16 row, UINT16 col,
                  UINT16 length, UINT16 width)
{
    UINT16 r;
    UINT16 c;
    UINT16 cur_row;
    UINT16 cur_col;
    UINT32 word_index;
    UINT32 bit_pos;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return;

    if (row + length > SCREEN_HEIGHT) length = SCREEN_HEIGHT - row;
    if (col + width  > SCREEN_WIDTH)  width  = SCREEN_WIDTH  - col;

    for (r = 0; r < length; r++)
    {
        for (c = 0; c < width; c++)
        {
            cur_row    = row + r;          /* was using c for both — fixed 
            cur_col    = col + c;
            word_index = cur_row * LONGS_PER_ROW + (cur_col / 32);
            bit_pos    = 31 - (cur_col % 32);
            base[word_index] &= ~(1UL << bit_pos);
        }
    }
}   /* was missing closing brace for outer loop — fixed */

/* ------------------------------------------------------------------ */
/* plot_pixel                                                           */
/* ------------------------------------------------------------------  */

void plot_pixel(UINT8 *base, UINT16 row, UINT16 col)
{
    UINT32 byte_index;  /* declared at top — C89 requirement */
    UINT8  bit_mask;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return;

    byte_index = (UINT32)row * BYTES_PER_ROW + (col / 8);
    bit_mask   = 1 << (7 - (col % 8));
    base[byte_index] |= bit_mask;
}

/* ------------------------------------------------------------------ */
/* plot_horizontal_line                                                 */
/* ------------------------------------------------------------------  */

void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col,
                          UINT16 length)
{
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  i;
    UINT32  byte_index;
    UINT8   bit_mask;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return;
    if (col + length > SCREEN_WIDTH) length = SCREEN_WIDTH - col;

    for (i = 0; i < length; i++)   /* was "0l" (letter l) — fixed to 0 */
    {
        byte_index = (UINT32)row * BYTES_PER_ROW + ((col + i) / 8);
        bit_mask   = 1 << (7 - ((col + i) % 8));
        byte_base[byte_index] |= bit_mask;
    }
}

/* ------------------------------------------------------------------ */
/* plot_vertical_line                                                   */
/* ------------------------------------------------------------------ */

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col,
                        UINT16 length)
{
    UINT8  *byte_base = (UINT8 *)base;
    UINT16  i;
    UINT32  byte_index;
    UINT8   bit_mask;

    if (row >= SCREEN_HEIGHT || col >= SCREEN_WIDTH) return;
    if (row + length > SCREEN_HEIGHT) length = SCREEN_HEIGHT - row;

    bit_mask = 1 << (7 - (col % 8));   /* constant per column — compute once */

    for (i = 0; i < length; i++)
    {
        byte_index = (UINT32)(row + i) * BYTES_PER_ROW + (col / 8);
        byte_base[byte_index] |= bit_mask;
    }
}

/* ------------------------------------------------------------------ */
/* plot_line  (Bresenham)                                               */
/* ------------------------------------------------------------------ */

void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col,
               UINT16 end_row, UINT16 end_col)
{
    int row = (int)start_row;
    int col = (int)start_col;
    int dr = (int)end_row - (int)start_row;
    int dc = (int)end_col - (int)start_col;
    int abs_dr = dr < 0 ? -dr : dr;
    int abs_dc = dc < 0 ? -dc : dc;
    int step_r = dr < 0 ? -1 : 1;
    int step_c = dc < 0 ? -1 : 1;
    int err = abs_dc - abs_dr;
    int e2;

    while (1)
    {
        plot_pixel((UINT8 *)base, (UINT16)row, (UINT16)col);
        if (row == (int)end_row && col == (int)end_col) break;
        e2 = 2 * err;
        if (e2 > -abs_dr) { err -= abs_dr; col += step_c; }
        if (e2 <  abs_dc) { err += abs_dc; row += step_r; }
    }
}
/* ------------------------------------------------------------------ */
/* plot_rectangle                                                       */
/* ------------------------------------------------------------------ 

void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col,
                    UINT16 length, UINT16 width)
{
    if (length == 0 || width == 0) return;

    plot_horizontal_line(base, row,            col, width);   /* top    
    plot_horizontal_line(base, row + length - 1, col, width); /* bottom 
    plot_vertical_line  (base, row, col,            length);  /* left   
    plot_vertical_line  (base, row, col + width - 1, length); /* right  
}

/* ------------------------------------------------------------------ */
/* plot_square                                                          */
/* ------------------------------------------------------------------ */

void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side)
{
    if (side == 0) return;
    plot_rectangle(base, row, col, side, side);
}

/* ------------------------------------------------------------------ */
/* plot_triangle                                                        */
/* ------------------------------------------------------------------ */

void plot_triangle(UINT32 *base, UINT16 row, UINT16 col,
                   UINT16 base_len, UINT16 height, UINT8 direction)
{
    switch (direction)
    {
        case 0: /* anchor = top-left */
            plot_horizontal_line(base, row, col, base_len);
            plot_vertical_line  (base, row, col, height);
            plot_line(base, row + height - 1, col,
                            row,             col + base_len);
            break;

        case 1: /* anchor = top-right */
            plot_horizontal_line(base, row, col - base_len + 1, base_len);
            plot_vertical_line  (base, row, col, height);
            plot_line(base, row + height - 1, col,   /* was plot() — fixed */
                            row, col - base_len + 1);
            break;

        case 2: /* anchor = bottom-left */
            plot_horizontal_line(base, row, col, base_len);
            plot_vertical_line  (base, row - height + 1, col, height);
            plot_line(base, row - height + 1, col,
                            row, col + base_len - 1);
            break;

        case 3: /* anchor = bottom-right */
            plot_horizontal_line(base, row, col - base_len + 1, base_len);
            plot_vertical_line  (base, row - height + 1, col, height);
            plot_line(base, row - height + 1, col,
                            row, col - base_len + 1);
            break;
    }
}

/* ------------------------------------------------------------------ */
/* plot_bitmap_8                                                        */
/* ------------------------------------------------------------------ */

void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col,
                   const UINT8 *bitmap, UINT16 height)
{
    UINT16 r;
    UINT16 shift     = col % 8;
    UINT16 byte_col  = col / 8;
    UINT32 screen_index;
    UINT8  data;

    for (r = 0; r < height; r++)
    {
        screen_index = (UINT32)(row + r) * BYTES_PER_ROW + byte_col;
        data         = bitmap[r];

        if (shift == 0)
        {
            base[screen_index] |= data;
        }
        else
        {
            base[screen_index]     |= (data >> shift);
            base[screen_index + 1] |= (data << (8 - shift));  /* spill */
        }
    }
}

/* ------------------------------------------------------------------ */
/* plot_bitmap_16                                                       */
/* ------------------------------------------------------------------ */

void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col,
                    const UINT16 *bitmap, UINT16 height)
{
    UINT16 r;
    UINT16 shift       = col % 16;
    UINT16 word_col    = col / 16;
    UINT32 screen_index;
    UINT16 data;

    for (r = 0; r < height; r++)
    {
        screen_index = (UINT32)(row + r) * WORDS_PER_ROW + word_col;
        data         = bitmap[r];

        if (shift == 0)
        {
            base[screen_index] |= data;
        }
        else
        {
            base[screen_index]     |= (data >> shift);
            base[screen_index + 1] |= (data << (16 - shift)); /* was writing to screen_index again — fixed */
        }
    }
}

/* ------------------------------------------------------------------ */
/* plot_bitmap_32                                                       */
/* ------------------------------------------------------------------ */

void plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col,
                    const UINT32 *bitmap, UINT16 height)
{
    UINT16 r;
    UINT16 shift       = col % 32;
    UINT16 word_col    = col / 32;
    UINT32 screen_index;
    UINT32 data;          /* was "UINT data" — fixed to UINT32 */

    for (r = 0; r < height; r++)
    {
        screen_index = (UINT32)(row + r) * LONGS_PER_ROW + word_col;
        data         = bitmap[r];

        if (shift == 0)
        {
            base[screen_index] |= data;
        }
        else
        {
            base[screen_index]     |= (data >> shift);
            base[screen_index + 1] |= (data << (32 - shift));
        }
    }
}

/* ------------------------------------------------------------------ */
/* plot_character                                                       */
/*                                                                      */
/* Font table layout (TOS / 8x8 convention used here):                 */
/*   Row N of char C is at font_table[C * 8 + N]                       */
/* Call linea0() once at program start and pass (UINT8 *)V_FNT_AD.     */
/* ------------------------------------------------------------------ */

void plot_character(UINT8 *base, UINT16 row, UINT16 col,
                    char ch, const UINT8 *font_table)
{
    UINT8        glyph[8];
    UINT16       r;
    const UINT8 *src = font_table + (unsigned char)ch * 8;

    for (r = 0; r < 8; r++)
        glyph[r] = src[r];

    plot_bitmap_8(base, row, col, glyph, 8); /* was "r < 0" — fixed to 8 */
}

/* ------------------------------------------------------------------ */
/* plot_string                                                          */
/* ------------------------------------------------------------------ */

void plot_string(UINT8 *base, UINT16 row, UINT16 col,
                 const char *str, const UINT8 *font_table)
{
    UINT16 current_col = col;

    while (*str != '\0')
    {
        plot_character(base, row, current_col, *str, font_table);
        current_col += 8;
        str++;
    }
}