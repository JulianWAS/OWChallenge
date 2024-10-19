#include <stdio.h>
#include "mock.h"
#include "PumpManager.h"
#include "ChannelManager.h"
#include "RunCycle.h"
#include "SerialParser.h"

int main(void)
{
	/* Init RTOS */
	if (RTOS_INIT() != ERROR_OK)
	{
		/* Handle the error */
		PANIC();
	}

	/* Init channel manager and channels*/
	if (channel_manager_init() != ERROR_OK)
	{
		/* Handle the error */
		PANIC();
	}

	/* Init pump manager and pump(s)*/
	if(pump_manager_init() != ERROR_OK)
	{
		/* Handle the error */
		PANIC();
	}

	/* Start the serial task */
	if(serial_parser_start() != ERROR_OK)
	{
		/* Handle the error */
		PANIC();
	}

	/* RTOS loop (assuming the choses RTOS needs one) */
	while (1)
	{
		/* Sleep mode power management entry if necessary */
	}
}