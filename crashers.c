#include "crashers.h"
#include "isr.h"

#define SCAN_M 0x32

static UINT8 screenBuffer[32255];

/*
 * Dispatches a single discrete key press (scan code) to the appropriate
 * async handler. Movement keys are NOT handled here - they are checked
 * each frame via is_key_held() in the main loop.
 *
 * Input:  model - pointer to the live game model
 *         scan  - scan code dequeued from keystroke()
 * Output: sets model->quit = TRUE if ESC is pressed
 */
void process_async_event(Model *model, UINT8 scan)
{
    switch (scan)
    {
        case SCAN_J:
            attack_swing_sound();
            on_light_attack(&model->player,
                            model->player.attack_cooldown);
            break;

        case SCAN_E:
            consume_potion(&model->player);
            break;

        case SCAN_ESC:
            model->quit = TRUE;
            break;

        default:
            break;
    }
}


/*
 * Function Purpose: Process all synch events
 * Updates positions, calculates and updates enemy velocity, etc.
 *
 * Input:  model - pointer to the live game model
 * Output: None, updates model fields
 * Assumptions: Time has been handled correctly to call this function
 */
void process_sync_events(Model *model)
{
    int i;
    /* Upate cooldowns and queue + player position */
    update_player_cooldowns(&model->player);
    update_player_position(&model->player);
    update_spawn_queue(model);

    /* Update all enemy positions and cooldowns */
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (model->enemy[i].active) {
            update_enemy_position(&model->enemy[i], &model->player);
            update_enemy_cooldown(&model->enemy[i]);
        }
    }
    /* Update boss events */
    if (model->boss.active) {
        update_boss_position(&model->boss, &model->player);
        update_boss_cooldown(&model->boss);
    }
    separate_enemies(model);
}


/* Function purpose: Processes all conditional (state-based) game events each tick.
 * Checks player-enemy and player-boss collisions, handles wave progression,
 * item drops and pickups, and evaluates win/lose conditions.
 * Input: The game model
 * Output: None, updates model state directly
 * Assumptions: process_sync_events has already run this tick so positions
 *              and cooldowns are current */
void process_cond_events(Model *model)
{
    int  i;
    bool player_died;
    /* attack enemies that are active */
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (!model->enemy[i].active) continue;

        player_hits_enemy(&model->player, &model->enemy[i]);
        /* hit player, check if dead */
        player_died = enemy_hits_player(&model->enemy[i], &model->player);
        if (player_died) /* if dead, quit */
        {
            model->quit = TRUE;
            return;
        }
    }
    /* repeat enemy logic for boss */
    if (model->boss.active)
    {
        player_hits_boss(&model->player, &model->boss);
        boss_summon(&model->boss, model);

        player_died = boss_hits_player(&model->boss, &model->player);
        if (player_died)
        {
            model->quit = TRUE;
            return;
        }
    }

    /* Item pickup: must run before next_level/drop_item so a freshly
     * dropped item cannot be grabbed the same tick it appears */
    for (i = 0; i < NUM_ITEMS; i++)
        grab_item(&model->player, &model->item[i]);

    if (!model->boss.active && next_level(model, model->stage))
    {
        drop_item(model, model->stage);
        model->stage++;
        /* Spawn waves */
        if (model->stage < 4)
            spawn_enemy(model, model->stage);
        else
            model->boss.active = TRUE;
    }

    model->healthbar.value = model->player.health;
    if (!model->quit)
        model->quit = level_end(model);
}

int main(void)
{
    /* set buffers, frame buffer, stack pointer, etc */
    void *orig_phys = Physbase();
    void *back_buf  = (void *)(((UINT32)screenBuffer + 255L) & ~255L);
    void *front_buf = orig_phys;
    void *temp;
    long old_ssp;
    Model model;
    UINT8 scan;

    /* Initialise model */
    init_model(&model);

    /* Initialise both buffers and render first frame */
    clear_screen(back_buf);
    render_reset();
    render(&model, back_buf);

    clear_screen(front_buf);
    render_reset();
    render(&model, front_buf);

    start_music();

    /* Install ISRs before entering any loop that depends on render_request */
    install_vectors();

    /* Flip to back buffer, then wait for the next VBL so the flip settles */
    old_ssp = Super(0);
    Setscreen(-1L, (long)back_buf, -1L);
    Super(old_ssp);

    render_request = 0;
    while (!render_request)
        ;
    render_request = 0;

    temp = front_buf;
    front_buf = back_buf;
    back_buf = temp;

    /* Splash screen loop */
    while (!model.started)
    {
        while (!render_request)
            ;
        render_request = 0;

        /* Drain keystroke buffer for splash navigation */
        while ((scan = keystroke()) != 0)
        {
            if (scan == SCAN_W)
                model.quit = FALSE;
            else if (scan == SCAN_S)
                model.quit = TRUE;
            else if (scan == SCAN_M)
                model.started = TRUE;
        }

        update_music(1);

        clear_screen(back_buf);
        render_reset();
        render(&model, back_buf);

        old_ssp = Super(0);
        Setscreen(-1L, (long)back_buf, -1L);
        Super(old_ssp);

        temp = front_buf;
        front_buf = back_buf;
        back_buf = temp;
    }

    /* Main game loop */
    while (!model.quit)
    {
        /* Wait for VBL tick */
        while (!render_request)
            ;
        render_request = 0;

        /* Continuous movement: checked every frame via held-key state */
        if (is_key_held(SCAN_W)) move_player(&model.player, 'w');
        if (is_key_held(SCAN_A)) move_player(&model.player, 'a');
        if (is_key_held(SCAN_S)) move_player(&model.player, 's');
        if (is_key_held(SCAN_D)) move_player(&model.player, 'd');
        if (is_key_held(SCAN_L)) move_player(&model.player, 'l');

        /* Discrete actions: drain the scan-code buffer */
        while ((scan = keystroke()) != 0)
            process_async_event(&model, scan);

        /* Synchronous and conditional events */
        update_music(1);
        process_sync_events(&model);
        process_cond_events(&model);

        /* Render current state into back buffer */
        clear_screen(back_buf);
        render_reset();
        render(&model, back_buf);

        /* Flip buffers - next iteration waits on render_request before
         * we touch back_buf again, so no explicit VBL poll needed here */
        old_ssp = Super(0);
        Setscreen(-1L, (long)back_buf, -1L);
        Super(old_ssp);

        temp = front_buf;
        front_buf = back_buf;
        back_buf = temp;
    }

    /* Restore original screen and OS state */
    stop_sound();
    uninstall_vectors();

    old_ssp = Super(0);
    Setscreen(-1L, (long)orig_phys, -1L);
    Super(old_ssp);

    return 0;
}