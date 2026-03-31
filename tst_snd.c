/*Test all sound implementations, which includes psg, music and effects*/

#include <osbind.h>
#include <stdio.h>
#include "psg.h"
#include "music.h"
#include "effects.h"

#define NOTE_A4   284

UINT8 result;

static volatile long *timer = (volatile long *)0x462L;

UINT32 get_time(void){
    UINT32 currTime;
    long oldSsp;
    oldSsp = Super(0);
    currTime = *timer;
    Super(oldSsp);
    return currTime;
}

UINT8 result;

void delay(long ticks){
    volatile long i;
    for (i = 0; i < ticks; i++);
}

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
    UINT8 result;    print_separator("write_psg / read_psg");

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

  /* ---------------------------------------
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
/* ------------------------
 * TEST: set_noise
 * ------------------------ */

void test_set_noise(){
    UINT8 result;
    print_separator("set_noise");

    /* Valid: minimum period — highest pitched noise */
    printf("[set_noise] period=0 (min, highest pitch)\n");
    set_volume(0, 10);
    enable_channel(0, 0, 1);
    set_noise(0);
    result = read_psg(6);
    printf("[set_noise] Reg 6: expected 0x00, got 0x%02X -- %s\n",
        result, result == 0x00 ? "PASS" : "FAIL");

    /* Valid: mid period */
    printf("[set_noise] period=15 (mid)\n");
    set_noise(15);
    result = read_psg(6);
    printf("[set_noise] Reg 6: expected 0x0F, got 0x%02X -- %s\n",
        result, result == 0x0F ? "PASS" : "FAIL");

    /* Valid: maximum period — lowest pitched noise */
    printf("[set_noise] period=31 (max, lowest pitch)\n");
    set_noise(31);
    result = read_psg(6);
    printf("[set_noise] Reg 6: expected 0x1F, got 0x%02X -- %s\n",
        result, result == 0x1F ? "PASS" : "FAIL");

    /* Invalid: over max — should be rejected */
    printf("[set_noise] period=32 (invalid, expect no change)\n");
    set_noise(31);               /* set known value first */
    set_noise(32);               /* attempt invalid */
    result = read_psg(6);
    printf("[set_noise] Reg 6: expected 0x1F unchanged, got 0x%02X -- %s\n",
        result, result == 0x1F ? "PASS" : "FAIL");

    /* Invalid: negative — should be rejected */
    printf("[set_noise] period=-1 (invalid, expect no change)\n");
    set_noise(15);               /* set known value first */
    set_noise(-1);               /* attempt invalid */
    result = read_psg(6);
    printf("[set_noise] Reg 6: expected 0x0F unchanged, got 0x%02X -- %s\n",
        result, result == 0x0F ? "PASS" : "FAIL");

    stop_sound();
}

/* -----------------------
 * TEST: set_envelope
 * ----------------------- */

void test_set_envelope(){
    UINT8 result_lo, result_hi, result_shape;
    print_separator("set_envelope");

    /* Valid: shape 8 = repeated decay, slow period */
    printf("[set_envelope] shape=8, period=5000\n");
    set_envelope(8, 5000);
    result_lo    = read_psg(11);
    result_hi    = read_psg(12);
    result_shape = read_psg(13);
    printf("[set_envelope] Reg 11 (lo): expected 0x%02X, got 0x%02X -- %s\n",
        (5000 & 0xFF), result_lo,
        result_lo == (5000 & 0xFF) ? "PASS" : "FAIL");
    printf("[set_envelope] Reg 12 (hi): expected 0x%02X, got 0x%02X -- %s\n",
        (5000 >> 8), result_hi,
        result_hi == (5000 >> 8) ? "PASS" : "FAIL");
    printf("[set_envelope] Reg 13 (shape): expected 0x08, got 0x%02X -- %s\n",
        result_shape, result_shape == 8 ? "PASS" : "FAIL");

    /* Valid: shape 14 = attack/decay repeat, fast period */
    printf("[set_envelope] shape=14, period=500\n");
    set_envelope(14, 500);
    result_lo    = read_psg(11);
    result_hi    = read_psg(12);
    result_shape = read_psg(13);
    printf("[set_envelope] Reg 11 (lo): expected 0x%02X, got 0x%02X -- %s\n",
        (500 & 0xFF), result_lo,
        result_lo == (500 & 0xFF) ? "PASS" : "FAIL");
    printf("[set_envelope] Reg 12 (hi): expected 0x%02X, got 0x%02X -- %s\n",
        (500 >> 8), result_hi,
        result_hi == (500 >> 8) ? "PASS" : "FAIL");
    printf("[set_envelope] Reg 13 (shape): expected 0x0E, got 0x%02X -- %s\n",
        result_shape, result_shape == 14 ? "PASS" : "FAIL");

    /* Boundary: minimum period */
    printf("[set_envelope] shape=12, period=0 (min period)\n");
    set_envelope(12, 0);
    result_lo = read_psg(11);
    result_hi = read_psg(12);
    printf("[set_envelope] Reg 11 (lo): expected 0x00, got 0x%02X -- %s\n",
        result_lo, result_lo == 0x00 ? "PASS" : "FAIL");
    printf("[set_envelope] Reg 12 (hi): expected 0x00, got 0x%02X -- %s\n",
        result_hi, result_hi == 0x00 ? "PASS" : "FAIL");

    /* Boundary: maximum period */
    printf("[set_envelope] shape=12, period=0xFFFF (max period)\n");
    set_envelope(12, 0xFFFF);
    result_lo = read_psg(11);
    result_hi = read_psg(12);
    printf("[set_envelope] Reg 11 (lo): expected 0xFF, got 0x%02X -- %s\n",
        result_lo, result_lo == 0xFF ? "PASS" : "FAIL");
    printf("[set_envelope] Reg 12 (hi): expected 0xFF, got 0x%02X -- %s\n",
        result_hi, result_hi == 0xFF ? "PASS" : "FAIL");

    /* Invalid: shape over max */
    printf("[set_envelope] shape=16 (invalid, expect no change)\n");
    set_envelope(8, 1000);       /* set known state first */
    set_envelope(16, 1000);      /* attempt invalid shape */
    result_shape = read_psg(13);
    printf("[set_envelope] Reg 13: expected 0x08 unchanged, got 0x%02X -- %s\n",
        result_shape, result_shape == 8 ? "PASS" : "FAIL");

    /* Invalid: negative shape */
    printf("[set_envelope] shape=-1 (invalid, expect no change)\n");
    set_envelope(8, 1000);       /* set known state first */
    set_envelope(-1, 1000);      /* attempt invalid */
    result_shape = read_psg(13);
    printf("[set_envelope] Reg 13: expected 0x08 unchanged, got 0x%02X -- %s\n",
        result_shape, result_shape == 8 ? "PASS" : "FAIL");

    /* Audible test: all 16 shapes back to back */
    printf("[set_envelope] Audible: cycling all 16 shapes...\n");
    {
        int s;
        for (s = 0; s <= 15; s++){
            printf("[set_envelope] shape=%d\n", s);
            set_tone(0, NOTE_A4);
            write_psg(8, 0x10);          /* enable envelope mode on channel A */
            enable_channel(0, 1, 0);
            set_envelope(s, 3000);
            delay(WHOLE * 2);            /* hold long enough to hear the shape */
            stop_sound();
            delay(HALF);                 /* gap between shapes */
        }
    }

    stop_sound();
}

/* ------------------------
 * TEST: music
 * ------------------------ */
void test_music(){
    UINT32 time_then, time_now, time_elapsed;
    int ticks = 0;
    print_separator("music");

    /* test start_music initializes correctly */
    printf("[music] calling start_music...\n");
    start_music();
    printf("[music] curr_r7_val = 0x%02X -- %s\n",
        curr_r7_val, (curr_r7_val & 0x01) == 0 ? "PASS channel A tone on" : "FAIL channel A tone off");

    /* let music play for a few seconds via update_music */
    printf("[music] running update_music for ~5 seconds...\n");
    time_then = get_time();

    while (ticks < 300){        /* time delay */
        time_now     = get_time();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0){
            update_music(time_elapsed);
            time_then = time_now;
            ticks += time_elapsed;
        }
    }

    /* verify channel A is still active */
    printf("[music] after 5 seconds: curr_r7_val = 0x%02X -- %s\n",
        curr_r7_val, (curr_r7_val & 0x01) == 0 ? "PASS still playing" : "FAIL channel went silent");

    /* test that song loops, run for another 5 seconds */
    printf("[music] running for another 5 seconds to verify loop...\n");
    ticks = 0;
    time_then = get_time();

    while (ticks < 300){
        time_now     = get_time();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0){
            update_music(time_elapsed);
            time_then = time_now;
            ticks += time_elapsed;
        }
    }

    printf("[music] loop test: curr_r7_val = 0x%02X -- %s\n",
        curr_r7_val, (curr_r7_val & 0x01) == 0 ? "PASS still playing" : "FAIL stopped after one loop");

    stop_sound();
    printf("[music] stop_sound called -- listen for silence\n");
}

/* ------------------------
 * TEST: attack_swing_sound
 * ------------------------ */
void test_attack_swing_sound(){
    print_separator("attack_swing_sound");

    attack_swing_sound();
    printf("[attack_swing] channel B tone on: %s\n",
        (curr_r7_val & 0x02) == 0 ? "PASS" : "FAIL");
    stop_sound();
}

/* ------------------------
 * TEST: on_hit_sound
 * ------------------------ */
void test_on_hit_sound(){
    print_separator("on_hit_sound");

    on_hit_sound();
    printf("[on_hit] channel B tone on: %s\n",
        (curr_r7_val & 0x02) == 0 ? "PASS" : "FAIL");
    stop_sound();
}

/* ------------------------
 * TEST: enemy_death_sound
 * ------------------------ */
void test_enemy_death_sound(){
    print_separator("enemy_death_sound");

    enemy_death_sound();
    printf("[enemy_death] channel C noise on: %s\n",
        (curr_r7_val & 0x20) == 0 ? "PASS" : "FAIL");
    stop_sound();
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
    test_set_noise();
    test_set_envelope();
    test_music();
    test_attack_swing_sound();
    test_on_hit_sound();
    test_enemy_death_sound();

    printf("\n========== ALL TESTS COMPLETE ==========\n");
    return 0;
}