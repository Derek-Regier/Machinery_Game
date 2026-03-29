#include <osbind.h>
#include "psg.h"
#include <stdio.h>

/* -----------------------
 *  HELPERS
 * -----------------------*/

void print_r7_bits(){
    printf("curr_r7_val = 0x%02X  [ C_noise:%d B_noise:%d A_noise:%d | C_tone:%d B_tone:%d A_tone:%d ]\n",
        curr_r7_val,
        (curr_r7_val >> 5) & 1,
        (curr_r7_val >> 4) & 1,
        (curr_r7_val >> 3) & 1,
        (curr_r7_val >> 2) & 1,
        (curr_r7_val >> 1) & 1,
        (curr_r7_val >> 0) & 1);
}

void print_separator(const char *label){
    printf("\n========== %s ==========\n", label);
}

  /* ---------------------------------------------
   * TEST: write_psg / read_psg
   * --------------------------------------------*/

void test_write_read_psg(){
    UINT8 result;
    print_separator("write_psg / read_psg");

    /* Test: write to tone register A (reg 0) and read back */
    write_psg(0, 0xAB);
    result = read_psg(0);
    printf("[write_psg/read_psg] Reg 0: wrote 0xAB, read 0x%02X -- %s\n",
        result, result == 0xAB ? "PASS" : "FAIL");

    /* Test: write 0 */
    write_psg(0, 0x00);
    result = read_psg(0);
    printf("[write_psg/read_psg] Reg 0: wrote 0x00, read 0x%02X -- %s\n",
        result, result == 0x00 ? "PASS" : "FAIL");

    /* Test: write max value */
    write_psg(0, 0xFF);
    result = read_psg(0);
    printf("[write_psg/read_psg] Reg 0: wrote 0xFF, read 0x%02X -- %s\n",
        result, result == 0xFF ? "PASS" : "FAIL");
}

  /* --------------------------------------------
   *  TEST: set_tone
   * ---------------------------------------*/

void test_set_tone(){
    print_separator("set_tone");

    /* Valid: middle tuning value, all channels */
    printf("[set_tone] Channel A, tuning=440 (0x1B8)\n");
    set_tone(0, 440);

    printf("[set_tone] Channel B, tuning=880 (0x370)\n");
    set_tone(1, 880);

    printf("[set_tone] Channel C, tuning=4095 (max)\n");
    set_tone(2, 4095);

    /* Boundary: tuning = 0 */
    printf("[set_tone] Channel A, tuning=0 (min)\n");
    set_tone(0, 0);

    /* Invalid: tuning over 4095, should silently return */
    printf("[set_tone] Channel A, tuning=4096 (invalid, expect no change)\n");
    set_tone(0, 4096);

    /* Invalid: bad channel */
    printf("[set_tone] Channel 3 (invalid, expect no change)\n");
    set_tone(3, 440);

    printf("[set_tone] Done. Verify registers 0-5 on hardware if possible.\n");
}

/* --------------------
 * TEST: set_volume
 * -------------------- */

void test_set_volume(){
    print_separator("set_volume");

    /* Valid range: 1-14 */
    printf("[set_volume] Channel A, volume=8\n");
    set_volume(0, 8);

    printf("[set_volume] Channel B, volume=1 (min valid)\n");
    set_volume(1, 1);

    printf("[set_volume] Channel C, volume=14 (max valid)\n");
    set_volume(2, 14);

    /* Boundary/invalid: 0 should be rejected */
    printf("[set_volume] Channel A, volume=0 (invalid, expect no change)\n");
    set_volume(0, 0);

    /* Boundary/invalid: 15 should be rejected */
    printf("[set_volume] Channel A, volume=15 (invalid, expect no change)\n");
    set_volume(0, 15);

    /* Invalid channel */
    printf("[set_volume] Channel 3 (invalid, expect no change)\n");
    set_volume(3, 8);
}

 /* ------------------------
  * TEST: enable_channel
  * ------------------------ */

void test_enable_channel(){
    print_separator("enable_channel");

    /* Reset to known state before each subtest */
    curr_r7_val = 0x3F;
    printf("\n-- All channels off (0x3F baseline) --\n");
    print_r7_bits();

    /* Turn tone ON for each channel */
    printf("\n-- Tone ON: Channel A --\n");
    curr_r7_val = 0x3F;
    enable_channel(0, 1, 0);
    print_r7_bits();
    printf("Expected bit 0 = 0: %s\n", (curr_r7_val & 0x01) == 0 ? "PASS" : "FAIL");

    printf("\n-- Tone ON: Channel B --\n");
    curr_r7_val = 0x3F;
    enable_channel(1, 1, 0);
    print_r7_bits();
    printf("Expected bit 1 = 0: %s\n", (curr_r7_val & 0x02) == 0 ? "PASS" : "FAIL");

    printf("\n-- Tone ON: Channel C --\n");
    curr_r7_val = 0x3F;
    enable_channel(2, 1, 0);
    print_r7_bits();
    printf("Expected bit 2 = 0: %s\n", (curr_r7_val & 0x04) == 0 ? "PASS" : "FAIL");

    /* Turn noise ON for each channel */
    printf("\n-- Noise ON: Channel A --\n");
    curr_r7_val = 0x3F;
    enable_channel(0, 0, 1);
    print_r7_bits();
    printf("Expected bit 3 = 0: %s\n", (curr_r7_val & 0x08) == 0 ? "PASS" : "FAIL");

    printf("\n-- Noise ON: Channel B --\n");
    curr_r7_val = 0x3F;
    enable_channel(1, 0, 1);
    print_r7_bits();
    printf("Expected bit 4 = 0: %s\n", (curr_r7_val & 0x10) == 0 ? "PASS" : "FAIL");

    printf("\n-- Noise ON: Channel C --\n");
    curr_r7_val = 0x3F;
    enable_channel(2, 0, 1);
    print_r7_bits();
    printf("Expected bit 5 = 0: %s\n", (curr_r7_val & 0x20) == 0 ? "PASS" : "FAIL");

    /* Both tone and noise ON */
    printf("\n-- Tone + Noise ON: Channel A --\n");
    curr_r7_val = 0x3F;
    enable_channel(0, 1, 1);
    print_r7_bits();
    printf("Expected bits 0,3 = 0: %s\n", (curr_r7_val & 0x09) == 0 ? "PASS" : "FAIL");

    /* Accumulated state: enable all three channels */
    printf("\n-- Enable all channels tone only --\n");
    curr_r7_val = 0x3F;
    enable_channel(0, 1, 0);
    enable_channel(1, 1, 0);
    enable_channel(2, 1, 0);
    print_r7_bits();
    printf("Expected bits 0,1,2 = 0: %s\n", (curr_r7_val & 0x07) == 0 ? "PASS" : "FAIL");

    /* Invalid channel — r7 should not change */
    printf("\n-- Invalid channel 3 (expect no change) --\n");
    curr_r7_val = 0x3F;
    enable_channel(3, 1, 1);
    print_r7_bits();
    printf("Expected 0x3F unchanged: %s\n", curr_r7_val == 0x3F ? "PASS" : "FAIL");
}

 /* -----------------------------
  * TEST: stop_sound
  * ----------------------------- */

void test_stop_sound(){
    print_separator("stop_sound");

    /* Set volumes high first */
    set_volume(0, 14);
    set_volume(1, 14);
    set_volume(2, 14);
    printf("[stop_sound] Volumes set to 14. Calling stop_sound()...\n");

    stop_sound();

    /* Read back and verify */
    printf("[stop_sound] Reg 8  (Ch A vol) = 0x%02X -- %s\n", read_psg(8),  read_psg(8)  == 0 ? "PASS" : "FAIL");
    printf("[stop_sound] Reg 9  (Ch B vol) = 0x%02X -- %s\n", read_psg(9),  read_psg(9)  == 0 ? "PASS" : "FAIL");
    printf("[stop_sound] Reg 10 (Ch C vol) = 0x%02X -- %s\n", read_psg(10), read_psg(10) == 0 ? "PASS" : "FAIL");
}

/* ---------
 * MAIN
 * ---------*/

int main(){
    test_write_read_psg();
    test_set_tone();
    test_set_volume();
    test_enable_channel();
    test_stop_sound();

    printf("\n========== ALL TESTS COMPLETE ==========\n");
    return 0;
}