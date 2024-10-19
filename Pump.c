#include <stdint.h>
#include <string.h>
#include "mock.h"
#include "Pump.h"

#define PUMP_TIMEOUUT_MS 60 * 1000

/* Local prototypes */
static ow_errors_t pump_set_shutoff(pump_t* this);
static void pump_shutoff_cb(void* data);


ow_errors_t pump_init(pump_t* this, uint8_t index, const char* name, const uint8_t gpio)
{
	/* Check flash for saved parameters */
	pump_t* saved_data = NULL; // replace with function to get saved data

	/* Copy saved parameters to this */
	if (saved_data != NULL)
	{
		/* Can do additional data checks from passed in values if wanted */
		memcpy(this, saved_data, sizeof(*this));
	}
	else
	{
		/* First time, or needed reset */
		this->gpio = gpio;
		this->index = index;
		this->actuation_count = 0;
		if (pump_set_name(this, name))
		{
			return PUMP_ERROR_INIT_FAILED;
		}
	}

	return ERROR_OK;
}

ow_errors_t pump_set_state(pump_t* this, bool state)
{
	/* Which pump state? */
	if (state)
	{
		
		/* Pump on */
		if (GPIO_SET(this->gpio, HIGH) == true)
		{
			/* Simple inc here but handle wrap at some point*/
			this->actuation_count++;

			/* Save updated info here */

			/* Start shutoff timer here */
			pump_set_shutoff(this);
		}
		else
		{
			/* actuation failed */
			return PUMP_ERROR_GENERAL;
		}
	}
	else
	{
		/* pump off */
		if (GPIO_SET(this->gpio, LOW))
		{
		}
		else
		{
			/* actuation failed */
			return PUMP_ERROR_GENERAL;
		}
	}

	return ERROR_OK;
}

ow_errors_t pump_set_name(pump_t* this, const char* name)
{
	if (!strncpy_s(this->name, PUMP_NAME_LENGTH, name, PUMP_NAME_LENGTH))
	{
		this->name[PUMP_NAME_LENGTH - 1] = 0;
		return ERROR_OK;
	}
	return PUMP_ERROR_GENERAL;
}

static ow_errors_t pump_set_shutoff(pump_t* this)
{
	/* Start a 60 second timer with callback to shut off pump, assumes return fits withh error scheme*/
	return START_TIMER(PUMP_TIMEOUT_MS, pump_shutoff_cb, this);
}

static void pump_shutoff_cb(void* data)
{
	/* Get the pump attached to this callback */
	pump_t* pump = (pump_t*)data;
	
	/* Turn it off */
	if (pump_set_state(pump, false))
	{
		/* Couldn't turn it off - panic / reset*/
		PANIC();
	}
}
