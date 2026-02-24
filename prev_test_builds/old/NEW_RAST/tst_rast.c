/*
 * tst_rast.c - Visual test driver for the raster library.
 *
 * Each test draws to a distinct region of the screen.
 * Press any key to advance through each test stage.
 * Excludes bitmap, character, and string tests (tested separately).
 *
 * Screen layout reminder: 640 wide x 400 tall, top-left = (0,0).
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include <osbind.h>
#include "raster.h"

/*
 * wait - halts until any key is pressed.
 */
static void wait(void)
{
    Cnecin();
}

int main()
{
    void *base = Physbase();

    /* ----------------------------------------------------------------
     * STAGE 1: clear_screen
     * Fills screen with garbage first so the clear is visible.
     * ---------------------------------------------------------------- */
    plot_horizontal_line(base, 100, 0, 640);
    plot_horizontal_line(base, 200, 0, 640);
    plot_horizontal_line(base, 300, 0, 640);
    wait();

    clear_screen(base);
    wait();

    /* ----------------------------------------------------------------
     * STAGE 2: plot_pixel
     * Draws a small cross of individual pixels near the top-left.
     * You should see 9 pixels in a 3x3 pattern around (20,20).
     * ---------------------------------------------------------------- */
    plot_pixel(base, 18, 20);
    plot_pixel(base, 19, 20);
    plot_pixel(base, 20, 18);
    plot_pixel(base, 20, 19);
    plot_pixel(base, 20, 20);
    plot_pixel(base, 20, 21);
    plot_pixel(base, 20, 22);
    plot_pixel(base, 21, 20);
    plot_pixel(base, 22, 20);

    /* Boundary pixels - corners of screen */
    plot_pixel(base, 0,   0);
    plot_pixel(base, 0,   639);
    plot_pixel(base, 399, 0);
    plot_pixel(base, 399, 639);

    /* Off-screen attempts - should be silently ignored */
    plot_pixel(base, 400, 0);
    plot_pixel(base, 0,   640);
    wait();

    /* ----------------------------------------------------------------
     * STAGE 3: plot_horizontal_line
     * Full-width line at row 50.
     * Short line (100px) at row 70, starting at col 100.
     * Clipped line that starts at col 600 and would run 200px off-screen.
     * ---------------------------------------------------------------- */
    clear_screen(base);

    plot_horizontal_line(base, 50,  0,   640);  /* full width          */
    plot_horizontal_line(base, 70,  100, 100);  /* short, middle       */
    plot_horizontal_line(base, 90,  600, 200);  /* clips at col 640    */
    wait();

    /* ----------------------------------------------------------------
     * STAGE 4: plot_vertical_line
     * Full-height line at col 100.
     * Short line (80px) at col 300, starting at row 50.
     * Clipped line that starts at row 380 and would run 100px off-screen.
     * ---------------------------------------------------------------- */
    clear_screen(base);

    plot_vertical_line(base, 0,   100, 400);  /* full height          */
    plot_vertical_line(base, 50,  300, 80);   /* short, middle        */
    plot_vertical_line(base, 380, 500, 100);  /* clips at row 400     */
    wait();

    /* ----------------------------------------------------------------
     * STAGE 5: plot_line (diagonal)
     * Four diagonals crossing at screen centre (200, 320).
     * ---------------------------------------------------------------- */
    clear_screen(base);

    plot_line(base, 0,   0,   399, 639);  /* top-left  to bottom-right */
    plot_line(base, 0,   639, 399, 0);    /* top-right to bottom-left  */
    plot_line(base, 0,   320, 399, 320);  /* vertical through centre   */
    plot_line(base, 200, 0,   200, 639);  /* horizontal through centre */
    wait();

    /* ----------------------------------------------------------------
     * STAGE 6: plot_rectangle
     * Large border rectangle near screen edges.
     * Medium rectangle in the upper-right quadrant.
     * Tiny 4x4 rectangle to check minimum size.
     * ---------------------------------------------------------------- */
    clear_screen(base);

    plot_rectangle(base, 10,  10,  380, 620);  /* near-full-screen border */
    plot_rectangle(base, 30,  400, 100, 200);  /* upper-right quadrant    */
    plot_rectangle(base, 50,  50,  4,   4);    /* tiny rectangle          */
    wait();

    /* ----------------------------------------------------------------
     * STAGE 7: plot_square
     * Three squares of different sizes.
     * ---------------------------------------------------------------- */
    clear_screen(base);

    plot_square(base, 50,  50,  200);  /* large  */
    plot_square(base, 100, 350, 80);   /* medium */
    plot_square(base, 300, 500, 20);   /* small  */
    wait();

    /* ----------------------------------------------------------------
     * STAGE 8: plot_triangle - all four anchor directions.
     * Each triangle is spaced into its own screen quadrant.
     *
     * direction 0: top-left anchor     (upper-left  quadrant)
     * direction 1: top-right anchor    (upper-right quadrant)
     * direction 2: bottom-left anchor  (lower-left  quadrant)
     * direction 3: bottom-right anchor (lower-right quadrant)
     * ---------------------------------------------------------------- */
    clear_screen(base);
    /*edge triangles*/
    plot_triangle(base, 20,  20,  120, 80, 0);   /* upper-left  */
    plot_triangle(base, 20,  619, 120, 80, 1);   /* upper-right */
    plot_triangle(base, 350, 20,  120, 80, 2);   /* lower-left  */
    plot_triangle(base, 350, 619, 120, 80, 3);   /* lower-right */
    /*middle triangles*/
    plot_triangle(base, 50, 121, 80, 120, 0);    /* upper-left  */
    plot_triangle(base, 50, 519, 80, 120, 1);    /* upper-right */
    plot_triangle(base, 300, 121,  80, 120, 2);  /* lower-left  */
    plot_triangle(base, 300, 519, 80, 120, 3);   /* lower-right */
    wait();

    /* ----------------------------------------------------------------
     * STAGE 9: clear_region
     * Draw a filled grid of horizontal lines, then clear a rectangular
     * chunk out of the middle to verify clear_region works.
     * ---------------------------------------------------------------- */
    clear_screen(base);

    plot_horizontal_line(base, 50,  0, 640);
    plot_horizontal_line(base, 100, 0, 640);
    plot_horizontal_line(base, 150, 0, 640);
    plot_horizontal_line(base, 200, 0, 640);
    plot_horizontal_line(base, 250, 0, 640);
    plot_horizontal_line(base, 300, 0, 640);
    wait();

    /* Clear a 150-tall x 300-wide hole in the centre of those lines */
    clear_region(base, 75, 170, 150, 300);
    wait();

    /* ----------------------------------------------------------------
     * STAGE 10: Final - clear and leave blank screen.
     * ---------------------------------------------------------------- */
    clear_screen(base);



    return 0;
}