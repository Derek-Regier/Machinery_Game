#include "crashers.h"

#define KEY_MOVE_UP    'w'
#define KEY_MOVE_DOWN  's'
#define KEY_MOVE_LEFT  'a'
#define KEY_MOVE_RIGHT 'd'
#define KEY_ATTACK     'j'
#define KEY_USE_ITEM   'e'
#define KEY_QUIT       '\x1B'

static UINT8 screenBuffer[32255];
static volatile long *timer = (volatile long *)0x462L;

UINT32 get_time(void)
{
    UINT32 t;
    long old_ssp;
    old_ssp = Super(0);
    t = *timer;
    Super(old_ssp);
    return t;
}

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

static void process_sync_events(Model *model)
{
    int i;
    update_player_cooldowns(&model->player);
    update_player_position(&model->player);
    for (i = 0; i < model->enemy_count; i++)
    {
        if (&model->enemy[0].active)
            update_enemy_position(&model->enemy[0]);
    }
}

static void process_cond_events(Model *model)
{
    int i;
    for (i = 0; i < model->enemy_count; i++)
    {
        if (model->enemy[i].active)
        {
            player_hits_enemy(&model->player, &model->enemy[i]);
            enemy_hits_player(&model->enemy[i], &model->player);
        }
    }
    if (model->boss.active)
    {
        player_hits_boss(&model->player, &model->boss);
        boss_hits_player(&model->boss, &model->player);
    }
    update_health_HUD(&model->player);
    model->quit = level_end(model);
}

int main(void)
{
    Model  model;
    UINT32 *base;
    UINT32 time_then;
    UINT32 time_now;
    UINT32 time_elapsed;
    char   key;

    base = (UINT32 *)(((UINT32)screenBuffer + 255) & ~255);

    init_model(&model);
    render(&model, base);

    time_then = get_time();

    while (!model.quit)
    {
        if (has_input())
        {
            key = get_input();
            process_async_event(&model, key);
        }

        time_now     = get_time();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0)
        {
            process_sync_events(&model);
            process_cond_events(&model);
            render(&model, base);
            time_then = time_now;
        }
    }

    reset_movement(&model);
    return 0;
}