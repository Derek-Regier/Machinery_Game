

/*Renamed file to castle_crashers.c*/
#include "crashers.h"

#define KEY_MOVE_UP 'w'
#define KEY_MOVE_DOWN 's'
#define KEY_MOVE_LEFT 'a'
#define KEY_MOVE_RIGHT 'd'
#define KEY_ATTACK 'j'
#define KEY_USE_ITEM 'e'
#define KEY_QUIT '\x1B' /* ESC */

UINT8 screenBuffer[32255]; /* allocated 32,000 Byte Buffer 256-Byte Alligned*/

UINT32 get_time(){
    long old_ssp;
    old_ssp = Super(0); /* enter privileged mode */
    timeNow = *timer;
    Super(old_ssp); /* exit privileged mode as soon as possible */
}

UINT32 timeThen, timeNow, timeElapsed;

timeNow = get_time();
timeElapsed = timeNow - timeThen;
if (timeElapsed > 0) {
    /* trigger synchronous events based on timeElapsed */
    /* render model */
    timeThen = timeNow;
}

/*
 * Routes a single keypress to the correct async handler.
 * Sets velocity/request fields ONLY - never moves game objects.
 * Sets model->quit on ESC so the main loop can exit cleanly.
 */
void process_async_event(Model *model, char key)
{
    switch (key)
    {
        case KEY_MOVE_UP:
        case KEY_MOVE_DOWN:
        case KEY_MOVE_LEFT:
        case KEY_MOVE_RIGHT:
            move_player(&model->player, key);
            break;

        case KEY_ATTACK:
            on_light_attack(&model->player,
                            model->player.attack_cooldown);
            break;

        case KEY_USE_ITEM:
            on_use_item(&model->player, &model->item[0]);
            break;

        case KEY_QUIT:
            model->quit = TRUE;
            break;

        default:
            break;
    }
}

/*
 * Process all synchronous (clock-driven) events.
 * Stub - to be implemented: move player, move enemy,
 * tick cooldowns, etc.
 */
static void process_sync_events(Model *model)
{
    int i;

    update_player_cooldowns(&model->player);
    update_player_position(&model->player);

    for (i = 0; i < model->enemy_count; i++)
    {
        if (model->enemy[i].active)
        {
            update_enemy_position(&model->enemy[i]);
        }
    }
}

/*
 * Process all conditional events.
 * Stub - to be implemented: enemy attack trigger,
 * player death check, boss summon, etc.
 */
static void process_cond_events(Model *model)
{
    int  i;
    bool player_died;

    for (i = 0; i < model->enemy_count; i++)
    {
        if (!model->enemy[i].active) continue;

        player_hits_enemy(&model->player, &model->enemy[i]);

        player_died = enemy_hits_player(&model->enemy[i], &model->player);
        if (player_died)
        {
            player_dies(); /* Stub, may not even be needed given how we are doing stuff */
            model->quit = TRUE;
            return;  /* no point checking further */
        }
    }

    if (model->boss.active)
    {
        player_hits_boss(&model->player, &model->boss);
        boss_summon(&model->boss);

        player_died = boss_hits_player(&model->boss, &model->player);
        if (player_died)
        {
            player_dies();
            model->quit = TRUE;
            return;
        }
    }
    update_health_HUD(&model->player);
    model.quit = level_end(&model);

    /* TODO: check_enemy_attack, check_enemy_death, etc. */
}

int main(void)
{
    Model model;
    UINT32 time_then;
    UINT32 time_now;
    UINT32 time_elapsed;
    char key;

    init_model(&model);
    render(&model);

    model.quit = FALSE;
    time_then = get_time();

    while (!model.quit)
    {
        if (has_input())
        {
            key = get_input();
            process_async_event(&model, key);
        }

        time_now = get_time();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0)
        {
            process_sync_events(&model);
            process_cond_events(&model);
            render(&model);
            time_then = time_now;
        }
    }
    reset_movement(&model);

    return 0;
}

