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





#endif /* CRASHERS_H */
