/*
 * tst_evt.c - Test driver for asynch, synch, and cond event handlers.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include <stdio.h>
#include "asynch.h"
#include "synch.h"
#include "cond.h"
#include "model.h"

static int tests_run    = 0;
static int tests_passed = 0;

static void check(const char *desc, int result)
{
    tests_run++;
    if (result) {
        tests_passed++;
        printf("  PASS: %s\n", desc);
    } else {
        printf("  FAIL: %s\n", desc);
    }
}


/* Maker functions - build structs field by field */

static Player make_player(void)
{
    Player p;
    p.x              = 100;
    p.y              = 100;
    p.delta_x        = 0;
    p.delta_y        = 0;
    p.health         = 100;
    p.damage         = 10;
    p.is_attacking   = FALSE;
    p.item_cooldown  = 0;
    p.attack_cooldown = 0;
    p.h              = 32;
    p.w              = 32;
    return p;
}

static Enemy make_enemy(unsigned int x, unsigned int y, int health, int damage)
{
    Enemy e;
    e.x           = x;
    e.y           = y;
    e.delta_x     = 0;
    e.delta_y     = 0;
    e.health      = health;
    e.damage      = damage;
    e.w           = 32;
    e.h           = 32;
    e.is_attacking = FALSE;
    return e;
}

static Boss make_boss(unsigned int x, unsigned int y, int health, int max_health)
{
    Boss b;
    b.x          = x;
    b.y          = y;
    b.delta_x    = 0;
    b.delta_y    = 0;
    b.health     = health;
    b.max_health = max_health;
    b.damage     = 15;
    b.summoned   = FALSE;
    b.w          = 48;
    b.h          = 48;
    return b;
}

static Item make_item(int value)
{
    Item i;
    i.x     = 0;
    i.y     = 0;
    i.h     = 16;
    i.w     = 16;
    i.value = value;
    return i;
}
/* Async handler tests */

static void test_async_move(void)
{
    /*Test to ensure player velocity updates properly and does not modify position */
    Player p = make_player();
    printf("\nAsync: move_player\n");

    move_player(&p, 'd');
    check("'d' sets delta_x to +1", p.delta_x == 1);

    move_player(&p, 'a');
    check("'a' sets delta_x to -1", p.delta_x == -1);

    move_player(&p, 'w');
    check("'w' sets delta_y to +1", p.delta_y == 1);

    move_player(&p, 's');
    check("'s' sets delta_y to -1", p.delta_y == -1);

    check("x unchanged after async move_player", p.x == 100);
    check("y unchanged after async move_player", p.y == 100);
}

static void test_async_light_attack(void)
{
    /* Initialize two players and ensure is attacking protections and cooldowns work */
    Player p = make_player();
    Player p2 = make_player();
    unsigned int cd_before;
    printf("\nAsync: on_light_attack\n");

    on_light_attack(&p, 0);
    check("attack activates when cooldown arg is 0", player_is_attacking(&p));
    check("attack_cooldown set after activation",     p.attack_cooldown > 0);

    cd_before = p.attack_cooldown;
    on_light_attack(&p, 0);
    check("second attack ignored while cooldown active",
          p.attack_cooldown == cd_before);

    on_light_attack(&p2, 5);
    check("attack blocked when cooldown arg > 0", !player_is_attacking(&p2));
}

static void test_async_use_item(void)
{
    /* Initalize item and test if player's health updates according to the value */
    Player p = make_player();
    Item potion;
    int hp_before;
    printf("\nAsync: on_use_item\n");

    potion.x = 0;
    potion.y = 0;
    potion.h = 16;
    potion.w = 16;
    potion.value = 30;

    hp_before = p.health;
    on_use_item(&p, &potion);
    check("health increases by item value", p.health == hp_before + 30);
}

/* Synch handler tests */

static void test_synch_player_position(void)
{
    /* Test player movement updates properly */
    Player p = make_player();
    unsigned int x_before;
    unsigned int y_before;
    printf("\nSynch: update_player_position\n");

    p.delta_x = 3;
    p.delta_y = 2;
    update_player_position(&p);
    check("x advances by delta_x each tick", p.x == 103);
    check("y advances by delta_y each tick", p.y == 102);

    p.delta_x = 0;
    p.delta_y = 0;
    x_before  = p.x;
    y_before  = p.y;
    update_player_position(&p);
    check("zero delta_x leaves x unchanged", p.x == x_before);
    check("zero delta_y leaves y unchanged", p.y == y_before);
}

static void test_synch_screen_boundary(void)
{
    /* Test player does not move off screen from normal movement */
    Player p  = make_player();
    Player p2 = make_player();
    printf("\nSynch: screen boundary clamping\n");

    p.x = 607;
    p.y = 367;
    p.delta_x = 5;
    p.delta_y = 5;
    update_player_position(&p);
    check("x clamped at MAX_X (608)", p.x <= 608);
    check("y clamped at MAX_Y (368)", p.y <= 368);

    p2.x = 0;
    p2.y = 0;
    p2.delta_x = -5;
    p2.delta_y = -5;
    update_player_position(&p2);
    check("x clamped at 0 (left edge)", p2.x == 0);
    check("y clamped at 0 (top edge)",  p2.y == 0);
}

static void test_synch_cooldowns(void)
{
    /* test all cooldowns */
    Player p = make_player();
    printf("\nSynch: cooldown ticking\n");

    player_set_attacking(&p, TRUE, 3);
    check("is_attacking TRUE after set", player_is_attacking(&p));

    update_attack_cooldown(&p);
    check("cooldown decrements each tick", p.attack_cooldown == 2);

    update_attack_cooldown(&p);
    update_attack_cooldown(&p);
    check("is_attacking FALSE when cooldown expires", !player_is_attacking(&p));
    check("attack_cooldown reaches 0", p.attack_cooldown == 0);

    p.item_cooldown = 2;
    update_item_cooldown(&p);
    check("item_cooldown decrements", p.item_cooldown == 1);
    update_item_cooldown(&p);
    check("item_cooldown reaches 0", p.item_cooldown == 0);
    update_item_cooldown(&p);
    check("item_cooldown stays at 0", p.item_cooldown == 0);

    player_set_attacking(&p, TRUE, 2);
    p.item_cooldown = 2;
    update_player_cooldowns(&p);
    check("update_player_cooldowns ticks attack cd", p.attack_cooldown == 1);
    check("update_player_cooldowns ticks item cd",   p.item_cooldown   == 1);
}

static void test_synch_enemy_position(void)
{
    /* Test enemy movement */
    Enemy  e = make_enemy(200, 150, 50, 8);
    Player p = make_player();
    printf("\nSynch: update_enemy_position\n");

    e.delta_x = 2;
    e.delta_y = 1;
    update_enemy_position(&e, &p);
    check("enemy x advances by delta_x", e.x == 202);
    check("enemy y advances by delta_y", e.y == 151);
}

/* ------------------------------------------------------------ */
/* Conditional handler tests                                     */
/* ------------------------------------------------------------ */

static void test_cond_player_takes_damage(void)
{
    /* Test player taking damage from cond events */
    Player p = make_player();
    Player p2 = make_player();
    bool   died;
    printf("\nCond: player_take_damage\n");

    died = player_take_damage(&p, 30);
    check("health reduced by damage amount", p.health == 70);
    check("player not dead at 70 hp", !died);

    died = player_take_damage(&p, 70);
    check("player dies at exactly 0 hp", died);
    check("health is 0 or below", p.health <= 0);

    died = player_take_damage(&p2, 9999);
    check("overkill damage still returns died", died);
}

static void test_cond_player_hits_enemy(void)
{
    /* check variations of whether enemies get hit by player or not */
    Player p = make_player();
    Enemy e = make_enemy(110, 110, 50, 8);
    Enemy dead_e = make_enemy(110, 110, 0,  8);
    Enemy far_e  = make_enemy(400, 400, 50, 8);
    int hp_dead;
    printf("\nCond: player_hits_enemy\n");

    player_hits_enemy(&p, &e);
    check("no damage when player not attacking", e.health == 50);

    player_set_attacking(&p, TRUE, 5);
    player_hits_enemy(&p, &e);
    check("damage applied when attacking and overlapping",
          e.health == 50 - p.damage);

    hp_dead = dead_e.health;
    player_hits_enemy(&p, &dead_e);
    check("dead enemy not hit again", dead_e.health == hp_dead);

    player_hits_enemy(&p, &far_e);
    check("no damage when enemy not in hitbox range", far_e.health == 50);
}

static void test_cond_player_hits_boss(void)
{
    /* Similar tests as enemy */
    Player p = make_player();
    Boss boss = make_boss(110, 110, 200, 200);
    Boss far_boss = make_boss(500, 500, 200, 200);
    printf("\nCond: player_hits_boss\n");

    player_hits_boss(&p, &boss);
    check("no boss damage when not attacking", boss.health == 200);

    player_set_attacking(&p, TRUE, 5);
    player_hits_boss(&p, &boss);
    check("boss damaged when player attacks and overlaps",
          boss.health == 200 - p.damage);

    player_hits_boss(&p, &far_boss);
    check("no boss damage when out of range", far_boss.health == 200);
}

static void test_cond_enemy_hits_player(void)
{
    /* Test variations on enemy hitting player */
    Player p = make_player();
    Player p2 = make_player();
    Enemy e = make_enemy(110, 110, 50, 15);
    Enemy big;
    bool died;
    printf("\nCond: enemy_hits_player\n");

    died = enemy_hits_player(&e, &p);
    check("no player damage when enemy not attacking", p.health == 100);
    check("enemy_hits_player returns FALSE when not attacking", !died);

    e.is_attacking = TRUE;
    died = enemy_hits_player(&e, &p);
    check("player takes damage from attacking enemy",
          p.health == 100 - e.damage);
    check("not dead after one hit", !died);

    p2.health = 1;
    big = make_enemy(110, 110, 50, 999);
    big.is_attacking = TRUE;
    died = enemy_hits_player(&big, &p2);
    check("player_dies returned from lethal hit", died);
}

static void test_cond_boss_summon(void)
{
    /* Test if the boss summons properly and only once */
    Boss boss = make_boss(500, 200, 200, 200);
    printf("\nCond: boss_summon\n");

    boss_summon(&boss);
    check("summoned stays FALSE above threshold", !boss.summoned);

    boss.health = 100;
    boss_summon(&boss);
    check("summoned set to TRUE at threshold", boss.summoned);

    boss.health = 50;
    boss_summon(&boss);
    check("boss_summon does not re-fire after summoned set", boss.summoned);
}

static void test_cond_level_end(void)
{
    /* Test the game ending */
    Model model;
    printf("\nCond: level_end\n");

    init_model(&model);
    check("game continues at start", !level_end(&model));

    model.boss.health = 0;
    check("level ends when boss dies", level_end(&model));

    init_model(&model);
    model.player.health = 0;
    check("level ends when player dies", level_end(&model));
}

/* Main */

int main(void)
{
    /* Run all the tests */
    printf("Event Handler Test Driver\n");

    test_async_move();
    test_async_light_attack();
    test_async_use_item();

    test_synch_player_position();
    test_synch_screen_boundary();
    test_synch_cooldowns();
    test_synch_enemy_position();

    test_cond_player_takes_damage();
    test_cond_player_hits_enemy();
    test_cond_player_hits_boss();
    test_cond_enemy_hits_player();
    test_cond_boss_summon();
    test_cond_level_end();

    printf("Model Test Driver\n");

    test_player_movement();
    test_player_health();
    test_player_attack_state();
    test_player_hitbox();

    test_enemy_movement();
    test_enemy_health();
    test_enemy_hitbox();

    test_boss_health();
    test_boss_summon();
    test_boss_hitbox();

    test_item();
    test_model_init();

    printf("\nResults: %d / %d passed\n", tests_passed, tests_run);

    return (tests_passed == tests_run) ? 0 : 1;
}
