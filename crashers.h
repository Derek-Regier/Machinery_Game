#ifndef CRASHERS_H
#define CRASHERS_H

#include <stdio.h>
#include <osbind.h>
#include "render.h"
#include "raster.h"
#include "model.h"
#include "synch.h"
#include "asynch.h"
#include "cond.h"
#include "input.h"
#include "isr.h"
#include "types.h" 
#include "psg.h"
#include "music.h"
#include "effects.h"

/*
 * Dispatches a single discrete key press (scan code) to the appropriate
 * async handler. Movement keys are handled separately via is_key_held()
 * in the main loop and do NOT pass through this function.
 *
 * Input:  model - pointer to the live game model
 *         scan  - scan code dequeued from keystroke()
 * Output: sets model->quit = TRUE if ESC is pressed
 */
void process_async_event(Model *model, UINT8 scan);

/*
 * Function Purpose: Process all synch events
 * Updates positions, calculates and updates enemy velocity, etc.
 *
 * Input:  model - pointer to the live game model
 * Output: None, updates model fields
 * Assumptions: Time has been handled correctly to call this function
 */
void process_sync_events(Model *model);

/* Function purpose: Processes all conditional (state-based) game events each tick.
 * Checks player-enemy and player-boss collisions, handles wave progression,
 * item drops and pickups, and evaluates win/lose conditions.
 * Input: The game model
 * Output: None, updates model state directly
 * Assumptions: process_sync_events has already run this tick so positions
 *              and cooldowns are current */
void process_cond_events(Model *model);

#endif /* CRASHERS_H */