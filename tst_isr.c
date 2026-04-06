/*
 * tst_isr.c - Interactive test for interrupt-driven keyboard input.
 *
 * Installs the VBL and IKBD ISRs, then updates the display showing:
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

#define ROW_TITLE 20
#define ROW_HINT 40
#define ROW_SCAN 80
#define ROW_HELD_LBL 110
#define ROW_HELD_IND 130

#define CH 8
#define COL_L 8
#define COL_HEX (COL_L + 17 * CH)
#define COL_IND_0 COL_L

/* ------------------------------------------------------------------ */

static void byte_to_hex(UINT8 val, char *buf)
{
    static const char hex[] = "0123456789ABCDEF";
    buf[0] = '0';
    buf[1] = 'x';
    buf[2] = hex[(val >> 4) & 0xF];
    buf[3] = hex[val & 0xF];
    buf[4] = '\0';
}

static void draw_labels(UINT8 *base)
{
    plot_string(base, ROW_TITLE, COL_L, "ISR INPUT TEST", font);
    plot_string(base, ROW_HINT, COL_L, "Press keys. ESC to quit.", font);
    plot_string(base, ROW_SCAN, COL_L, "Last scan code:  ", font);
    plot_string(base, ROW_HELD_LBL, COL_L, "Game keys (held):", font);
    plot_string(base, ROW_HELD_IND, COL_L, "W   A   S   D   E   J   L",font);
}

static void draw_state(UINT8 *base, UINT8 last_scan)
{
    static const UINT8 scans[7] = {
        SCAN_W, SCAN_A, SCAN_S, SCAN_D, SCAN_E, SCAN_J, SCAN_L
    };
    char hex_buf[5];
    int col;
    int i;

    clear_region((UINT32 *)base, ROW_SCAN, COL_HEX, CH, 4 * CH);
    byte_to_hex(last_scan, hex_buf);
    plot_string(base, ROW_SCAN, COL_HEX, hex_buf, font);

    col = COL_IND_0;
    for (i = 0; i < 7; i++)
    {
        clear_region((UINT32 *)base, ROW_HELD_IND + CH + 2, col, CH, CH);
        plot_character(base, ROW_HELD_IND + CH + 2, col,
                       is_key_held(scans[i]) ? '*' : '-', font);
        col += 4 * CH;
    }
}


int main(void)
{
    UINT8 *base = (UINT8 *)Physbase();
    UINT8 last_scan = 0;
    UINT8 scan;
    int   dirty = 0;

    install_vectors();

    /* Discard any bytes the IKBD had queued before our ISR was installed */
    while (keystroke() != 0)
        ;

    clear_screen((UINT32 *)base);
    draw_labels(base);
    draw_state(base, last_scan);

    while (!is_key_held(SCAN_ESC))
    {
        /*
         * Only drain the keystroke buffer once per VBL (70 Hz).
         * This prevents hammering mask_ikbd/unmask_ikbd in a tight
         * spin loop, which spams the ACIA control register and
         * prevents subsequent interrupts from firing.
         */
        if (render_request)
        {
            render_request = 0;

            while ((scan = keystroke()) != 0)
            {
                last_scan = scan;
                dirty = 1;
            }

            if (dirty)
            {
                draw_state(base, last_scan);
                dirty = 0;
            }
        }
    }

    uninstall_vectors();
    clear_screen((UINT32 *)base);
    return 0;
}