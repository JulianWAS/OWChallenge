#include "stdio.h"
#include "PumpManager.h"
#include "ChannelManager.h"

/* Pump manager instance*/
pump_manager_t pump_manager;

/* Default pump names */
const char* pump_default_name[MAX_PUMPS] =
{
	"Pump 1",
};

/* Default GPIO definitions*/
const uint8_t pump_default_gpio[MAX_PUMPS] =
{
	11,
};

/* Initialise the pump manager */
ow_errors_t pump_manager_init(void)
{
	ow_errors_t error = ERROR_OK;
	/* Loop through the managed pumps */
	for (uint8_t pump_index = 0; pump_index < MAX_PUMPS; pump_index++)
	{
		pump_t* pump = &pump_manager.pump[pump_index];
		/* Initialise the pump passing in defaults for if pump is not already set up*/
		error = pump_init(pump, pump_index, pump_default_name[pump_index], pump_default_gpio[pump_index]);
		if (error)
		{
			/* An error occurred in pump setup */
			return error;
		}
	}

	return ERROR_OK;
}

/* Set all managed pumps to the same state */
ow_errors_t pump_manager_set_all_state(bool state)
{
	ow_errors_t error = ERROR_OK;
	/* Loop through the managed pumps */
	for (uint8_t pump_index = 0; pump_index < MAX_PUMPS; pump_index++)
	{
		/* Initialise the pump passing in defaults for if pump is not already set up*/
		error = pump_manager_set_index_state(pump_index, state);
		if (error)
		{
			/* An error occurred in pump setup */
			return error;
		}
	}

	return ERROR_OK;
}

/* Set a single managed pump to a state */
ow_errors_t pump_manager_set_index_state(pump_manager_index_t pump_index, bool state)
{
	/* Check pump is valid*/
	if (pump_index >= PUMP_1 && pump_index < PUMP_NOOF)
	{
		/* Set the new state of the pump*/
		pump_t* pump = &pump_manager.pump[pump_index];
		return pump_set_state(pump, state);
	}
	else
	{
		/* Pump was not valid */
		return PM_ERROR_GENERAL;
	}
}

/* Set the name of a pump */
ow_errors_t pump_manager_set_name(pump_manager_index_t pump_index, char* name)
{
	/* Check pump is valid*/
	if (pump_index >= PUMP_1 && pump_index < PUMP_NOOF)
	{
		/* Set the new state of the pump */
		pump_t* pump = &pump_manager.pump[pump_index];
		return pump_set_name(pump, name);
	}
	else
	{
		/* Pump was not valid */
		return PM_ERROR_GENERAL;
	}
}
