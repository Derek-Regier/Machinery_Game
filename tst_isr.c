/*
 * tst_isr.c - Interactive test for interrupt-driven keyboard input.
 *
 * Installs the VBL and IKBD ISRs, then redraws each VBL showing:
 *   - The last raw scan code received from keystroke()
 *   - The live held state of the game keys (W A S D E J L)
 *
 * Press ESC (scan 0x01) to exit cleanly.
 *
 * Build: make -f tst_isr.mak
 * Link:  isr.c  input.c  ISR_ASM.S  raster.c  RAST_ASM.S  font.c
 *
 * Authors: Derek Regier, Aydin Salonius, Chintan Thakor
 * Course:  COMP 2659, Winter 2026
 */

#include <osbind.h>
#include "TYPES.H"
#include "isr.h"
#include "input.h"
#include "raster.h"
#include "font.h"

/* Row positions (pixels) for each display line */
#define ROW_TITLE    20
#define ROW_HINT     40
#define ROW_SCAN     80
#define ROW_GAME    110
#define ROW_HELD    140

#define COL_L         8

/* ------------------------------------------------------------------ */
/* Helpers                                                             */
/* ------------------------------------------------------------------ */

/*
 * Writes "0xXX" for the given byte into buf (must be >= 5 bytes).
 */
static void byte_to_hex(UINT8 val, char *buf)
{
    static const char hex[] = "0123456789ABCDEF";
    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = hex[(val >> 4) & 0xF];
    buf[3] = hex[val & 0xF];
    buf[4] = '\0';
}

/*
 * Draws the static labels that never change.
 */
static void draw_labels(UINT8 *base)
{
    plot_string(base, ROW_TITLE, COL_L,
                "ISR INPUT TEST", font);
    plot_string(base, ROW_HINT,  COL_L,
                "Press keys. ESC to quit.", font);
    plot_string(base, ROW_SCAN,  COL_L,
                "Last scan code:  ", font);
    plot_string(base, ROW_GAME,  COL_L,
                "Game keys (held):", font);
    plot_string(base, ROW_HELD,  COL_L,
                "W   A   S   D   E   J   L", font);
}

/*
 * Redraws the dynamic portion of the display.
 */
static void draw_state(UINT8 *base, UINT8 last_scan)
{
    char hex_buf[5];
    int col;

    /* Last scan code */
    byte_to_hex(last_scan, hex_buf);
    plot_string(base, ROW_SCAN, COL_L + 17 * 8, hex_buf, font);

    /* Held indicators for each game key, spaced 4 chars apart to match
       the "W   A   S   D   E   J   L" label above */
    col = COL_L;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_W) ? '*' : '-', font);
    col += 4 * 8;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_A) ? '*' : '-', font);
    col += 4 * 8;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_S) ? '*' : '-', font);
    col += 4 * 8;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_D) ? '*' : '-', font);
    col += 4 * 8;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_E) ? '*' : '-', font);
    col += 4 * 8;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_J) ? '*' : '-', font);
    col += 4 * 8;

    plot_character(base, ROW_HELD + 12, col,
                   is_key_held(SCAN_L) ? '*' : '-', font);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */

int main(void)
{
    UINT8 *base = (UINT8 *)Physbase();
    UINT8 last_scan = 0;
    UINT8 scan;

    install_vectors();

    clear_screen((UINT32 *)base);
    draw_labels(base);
    draw_state(base, last_scan);

    while (!is_key_held(SCAN_ESC))
    {
        /* Drain the scan-code buffer; remember the most recent code */
        while ((scan = keystroke()) != 0)
        {
            last_scan = scan;
        }

        /* Redraw once per VBL */
        if (render_request)
        {
            render_request = 0;
            clear_screen((UINT32 *)base);
            draw_labels(base);
            draw_state(base, last_scan);
        }
    }

    uninstall_vectors();
    clear_screen((UINT32 *)base);
    return 0;
}