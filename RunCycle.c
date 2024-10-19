#include <stdio.h>
#include "PumpManager.h"
#include "RunCycle.h"

/* Start a run cyycle */
ow_errors_t run_cycle_start(void)
{
	/* TODO: Check at least 1 channel is open */

	/* Start the pumps */
	return pump_manager_set_all_state(true);
}