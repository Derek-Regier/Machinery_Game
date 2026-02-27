/*
 * tst_mdl.c - Test driver for Player, Enemy, Boss, and Item models.
 *
 * Verifies object behaviour functions in isolation using printf output.
 *
 * Authors: Aydin Salonius, Chintan Thakor, Derek Regier
 * Course: COMP 2659, Winter 2026
 */

#include <stdio.h>
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

/* ------------------------------------------------------------
 * Maker functions - build structs field by field so no
 * compound initializer is needed after a statement (C89 rule).
 * ------------------------------------------------------------ */

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

/* ------------------------------------------------------------ */
/* Player tests                                                  */
/* ------------------------------------------------------------ */

static void test_player_movement(void)
{
    Player p = make_player();
    printf("\nPlayer Movement\n");

    move_player_horizontal(&p);
    check("horizontal moves x by delta_x (no delta, stays)", p.x == 100);

    p.delta_x = 3;
    p.delta_y = 2;
    move_player_horizontal(&p);
    check("horizontal moves x by delta_x", p.x == 103);

    move_player_vertical(&p);
    check("vertical moves y by delta_y", p.y == 102);

    p.delta_x = -5;
    move_player_horizontal(&p);
    check("negative delta_x decrements x", p.x == 98);
}

static void test_player_health(void)
{
    Player p  = make_player();
    Player p2 = make_player();
    bool died;
    printf("\nPlayer Health\n");

    died = update_health(&p, -20);
    check("damage reduces health", p.health == 80);
    check("not dead at 80 hp", !died);

    died = update_health(&p, 10);
    check("healing increases health", p.health == 90);

    died = update_health(&p, -90);
    check("zero health returns dead", died);
    check("health at or below zero", p.health <= 0);

    died = update_health(&p2, -999);
    check("massive damage still returns dead", died);
}

static void test_player_attack_state(void)
{
    Player p = make_player();
    printf("\nPlayer Attack State\n");

    check("not attacking initially", !player_is_attacking(&p));

    player_set_attacking(&p, TRUE, 5);
    check("set_attacking activates when cooldown is 0", player_is_attacking(&p));
    check("cooldown set to 5", p.attack_cooldown == 5);

    player_set_attacking(&p, TRUE, 5);
    check("set_attacking ignored while cooldown active", p.attack_cooldown == 5);
}

static void test_player_hitbox(void)
{
    Player p = make_player();
    printf("\nPlayer Hitbox\n");

    check("overlap detected when rects intersect",
          player_hitbox_overlaps(&p, 110, 110, 32, 32));

    check("no overlap when rects only touch edge",
          !player_hitbox_overlaps(&p, 132, 100, 32, 32));

    check("no overlap when rects are far apart",
          !player_hitbox_overlaps(&p, 300, 300, 32, 32));

    check("overlap when one rect fully inside other",
          player_hitbox_overlaps(&p, 105, 105, 10, 10));
}

/* ------------------------------------------------------------ */
/* Enemy tests                                                   */
/* ------------------------------------------------------------ */

static void test_enemy_movement(void)
{
    Enemy e = make_enemy(200, 150, 50, 8);
    printf("\nEnemy Movement\n");

    e.delta_x = -2;
    e.delta_y =  3;
    move_enemy_horizontal(&e);
    check("enemy horizontal move by delta_x", e.x == 198);

    move_enemy_vertical(&e);
    check("enemy vertical move by delta_y", e.y == 153);
}

static void test_enemy_health(void)
{
    Enemy e = make_enemy(0, 0, 30, 8);
    bool died;
    printf("\nEnemy Health\n");

    died = update_enemy_health(&e, -10);
    check("damage reduces enemy health", e.health == 20);
    check("enemy not dead at 20 hp", !died);

    died = update_enemy_health(&e, -20);
    check("enemy dies at 0 hp", died);
    check("enemy health at or below zero", e.health <= 0);
}

static void test_enemy_hitbox(void)
{
    Enemy e = make_enemy(200, 200, 50, 8);
    printf("\nEnemy Hitbox\n");

    check("enemy overlap detected",
          enemy_hitbox_overlaps(&e, 210, 210, 32, 32));

    check("no overlap when enemy is far away",
          !enemy_hitbox_overlaps(&e, 400, 400, 32, 32));

    check("no overlap when touching left edge only",
          !enemy_hitbox_overlaps(&e, 168, 200, 32, 32));
}

/* ------------------------------------------------------------ */
/* Boss tests                                                    */
/* ------------------------------------------------------------ */

static void test_boss_health(void)
{
    Boss boss = make_boss(500, 200, 200, 200);
    bool died;
    printf("\nBoss Health\n");

    died = update_boss_health(&boss, -50);
    check("boss health reduced by 50", boss.health == 150);
    check("boss not dead at 150 hp", !died);

    died = update_boss_health(&boss, -150);
    check("boss dies at 0 hp", died);
}

static void test_boss_summon(void)
{
    Boss boss = make_boss(500, 200, 200, 200);
    printf("\nBoss Summon Threshold\n");

    check("summon returns FALSE above 50% health", !summon(&boss));

    boss.health = 100;
    check("summon returns TRUE at 50% health", summon(&boss));

    boss.health = 99;
    check("summon returns TRUE below 50% health", summon(&boss));
}

static void test_boss_hitbox(void)
{
    Boss boss = make_boss(500, 200, 200, 200);
    printf("\nBoss Hitbox\n");

    check("boss overlap detected with overlapping rect",
          boss_hitbox_overlaps(&boss, 510, 210, 32, 32));

    check("no overlap when rect is far away",
          !boss_hitbox_overlaps(&boss, 100, 100, 32, 32));
}

/* ------------------------------------------------------------ */
/* Item tests                                                    */
/* ------------------------------------------------------------ */

static void test_item(void)
{
    Item potion = make_item(30);
    Item empty  = make_item(0);
    printf("\nItem\n");

    check("get_value returns item value", get_value(&potion) == 30);
    check("get_value returns 0 for empty item", get_value(&empty) == 0);
}

/* ------------------------------------------------------------ */
/* Model init test                                               */
/* ------------------------------------------------------------ */

static void test_model_init(void)
{
    Model model;
    printf("\nModel Init\n");

    init_model(&model);

    check("player health initialized to 100", model.player.health == 100);
    check("player not attacking at start",    !model.player.is_attacking);
    check("enemy health initialized",         model.enemy[0].health > 0);
    check("boss health initialized",          model.boss.health > 0);
    check("boss not summoned at start",       !model.boss.summoned);
    check("boss max_health matches health",
          model.boss.max_health == model.boss.health);
    check("item value initialized",           model.item[0].value > 0);
}

/* ------------------------------------------------------------ */
/* Main                                                          */
/* ------------------------------------------------------------ */

int main(void)
{
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
