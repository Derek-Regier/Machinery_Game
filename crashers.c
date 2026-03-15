/*Renamed file to castle_crashers.c*/
#include "crashers.h"

#define KEY_MOVE_UP 'w'
#define KEY_MOVE_DOWN 's'
#define KEY_MOVE_LEFT 'a'
#define KEY_MOVE_RIGHT 'd'
#define KEY_ATTACK 'j'
#define KEY_USE_ITEM 'e'
#define KEY_QUIT '\x1B' /* ESC */

static UINT8 screenBuffer[32255];
static volatile long *timer = (volatile long *)0x462L;

UINT32 get_time(void)
{
    UINT32 currTime;
    long   oldSsp;

    oldSsp   = Super(0);
    currTime = *timer;
    Super(oldSsp);

    return currTime;
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

void process_sync_events(Model *model)
{
    int i;

    update_player_cooldowns(&model->player);
    update_player_position(&model->player);

    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (model->enemy[i].active) {
            update_enemy_position(&model->enemy[i], &model->player);
            update_enemy_cooldown(&model->enemy[i]);
        }
    }

    if (model->boss.active)
        update_boss_position(&model->boss, &model->player);
    separate_enemies(model);
}

void process_cond_events(Model *model)
{
    int  i;
    bool player_died;

    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (!model->enemy[i].active) continue;

        player_hits_enemy(&model->player, &model->enemy[i]);

        player_died = enemy_hits_player(&model->enemy[i], &model->player);
        if (player_died)
        {
            player_dies();
            model->quit = TRUE;
            return;
        }
    }

    if (model->boss.active)
    {
        player_hits_boss(&model->player, &model->boss);
        boss_summon(&model->boss, model);

        player_died = boss_hits_player(&model->boss, &model->player);
        if (player_died)
        {
            player_dies();
            model->quit = TRUE;
            return;
        }
    }

    if (!model->boss.active && next_level(model, model->stage))
    {
        drop_item(model, model->stage);
        model->stage++;

        if (model->stage < 4)
            spawn_enemy(model, model->stage);
        else
            model->boss.active = TRUE;
    }

    model->healthbar.value = model->player.health;
    model->quit = level_end(model);
}

int main(void)
{
    void   *orig_phys = Physbase();
    void   *back_buf  = (void *)(((UINT32)screenBuffer + 255L) & ~255L);
    void   *front_buf = orig_phys;
    void   *temp;
    long    old_ssp;
    long    vbl_now;
    Model   model;
    UINT32  time_then;
    UINT32  time_now;
    UINT32  time_elapsed;
    char    key;

    init_model(&model);

    /* render first frame into back buffer */
    clear_screen(back_buf);
    render(&model, back_buf);

    /* flip to back buffer, wait for VBL inside Super */
    old_ssp = Super(0);
    Setscreen(-1L, (long)back_buf, -1L);
    vbl_now = *timer;
    while (*timer == vbl_now)
        ;
    Super(old_ssp);

    /* swap so front is now displayed back_buf, back is orig_phys */
    temp = front_buf;
    front_buf = back_buf;
    back_buf = temp;

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

            /* clear and render into back buffer (not being displayed) */
            clear_screen(back_buf);
            render_reset();
            render(&model, back_buf);   

            /* flip back buffer to screen, wait for VBL */
            old_ssp = Super(0);
            Setscreen(-1L, (long)back_buf, -1L);
            vbl_now = *timer;
            while (*timer == vbl_now)
                ;
            Super(old_ssp);

            /* swap buffers */
            temp      = front_buf;
            front_buf = back_buf;
            back_buf  = temp;

            time_then = time_now;
        }
    }

    /* restore original physbase */
    old_ssp = Super(0);
    Setscreen(-1L, (long)orig_phys, -1L);
    Super(old_ssp);

    reset_movement(&model);
    return 0;
}