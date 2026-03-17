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
#include "types.h"

/*
 * Reads and returns the current 70Hz TOS timer value at 0x462.
 * Enters privileged mode only long enough to read the longword,
 * then immediately exits. Returns the tick count as UINT32.
 */
UINT32 get_time(void);

/*
 * Dispatches a single key press to the appropriate async handler.
 * Updates request/velocity fields on the model only - does NOT
 * move anything. Movement happens on the clock tick in synch.
 *
 * Input:  model - pointer to the live game model
 *         key   - character read from get_input()
 * Output: sets model->quit = TRUE if ESC is pressed
 */
void process_async_event(Model *model, char key);


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
