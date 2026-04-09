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
    long old_ssp = Super(0);   /* enter supervisor mode for program lifetime */

    /* set buffers, frame buffer, stack pointer, etc */
    void *orig_phys = (void *)get_video_base();
    void *back_buf  = (void *)(((UINT32)screenBuffer + 255L) & ~255L);
    void *front_buf = orig_phys;
    void *temp;
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

    /* Register model pointer so the VBL ISR can run game logic */
    set_isr_model(&model);

    /* Install ISRs before entering any loop that depends on render_request */
    install_vectors();

    /* Flip to back buffer, then wait for the next VBL so the flip settles */
    set_video_base(back_buf);

    render_request = 0;
    while (!render_request)
        ;
    render_request = 0;

    temp = front_buf;
    front_buf = back_buf;
    back_buf = temp;

    /* Splash screen loop */
    while (!model.started && !model.quit)
    {
        if (render_request)
        {
            int mx;
            int my;
            int quit_hovered;
            int clicked;

            mx = get_mouse_x();
            my = get_mouse_y();

            /* Midpoint sits in the gap between the two boxes (rows 214-222) */
            quit_hovered = (my >= 218);
            clicked      = get_mouse_button();

            /* Set model.quit temporarily so render_splash highlights the
             * correct option, then restore it — the loop condition must not
             * be affected by hover alone. */
            model.quit = quit_hovered;
            clear_screen(back_buf);
            render_reset();
            render(&model, back_buf);
            model.quit = FALSE;

            /* Commit selection only on click */
            if (clicked)
            {
                if (!quit_hovered)
                    model.started = TRUE;
                else
                    model.quit = TRUE;
            }

            set_video_base(back_buf);

            temp = front_buf;
            front_buf = back_buf;
            back_buf = temp;
            render_request = 0;
        }
    }

    /* Main game loop */
    while (!model.quit)
    {
        /* If asynchronous (discrete) event(s) outstanding, process them */
        if (has_keystroke())
        {
            while ((scan = keystroke()) != 0)
                process_async_event(&model, scan);
        }

        /* If the VBL ISR has signalled a new frame, render it */
        if (render_request)
        {
            /* Render current model state into back buffer */
            clear_screen(back_buf);
            render_reset();
            render(&model, back_buf);

            /* Schedule page flip */
            set_video_base(back_buf);

            temp = front_buf;
            front_buf = back_buf;
            back_buf = temp;
            render_request = 0;  /* clear AFTER scheduling flip */
        }
    }

    /* Restore original screen and OS state */
    stop_sound();
    uninstall_vectors();

    set_video_base(orig_phys);

    Super(old_ssp);
    return 0;
}