#include <stdio.h>
#include "ChannelManager.h"

/* Instance of the channel manager - prefer static allocation rather than heap due to constrained system*/
channel_manager_t channel_manager;

/* Default channel names const to force allocation in flash*/
const char* channel_default_name[MAX_SCENT_CHANNELS] =
{
	"Chemical 1",
	"Chemical 2",
	"Chemical 3",
	"Chemical 4",
	"Chemical 5",
	"Chemical 6",
};

/* Default GPIO definitions*/
const uint8_t channel_default_gpio[MAX_SCENT_CHANNELS] =
{
	46,
	3,
	8,
	18,
	17,
	16
};

/* Initialise all the channels within the manager*/
ow_errors_t channel_manager_init(void)
{
	ow_errors_t error = CM_ERROR_OK;
	/* Loop through the managed channels */
	for (uint8_t channel_index = 0; channel_index < MAX_SCENT_CHANNELS; channel_index++)
	{
		scent_channel_t* channel = &channel_manager.channel[channel_index];
		/* Initialise the chhannel passing in defaults for if channel is not already set up*/
		error = scent_channel_init(channel, channel_index, channel_default_name[channel_index], channel_default_gpio[channel_index]);
		if (error)
		{
			/* An error occurred in channel setup */
			return error;
		}
	}

	return ERROR_OK;
}

/* Set a channels state */
ow_errors_t channel_manager_set_channel_state(channel_manager_index_t channel_index, bool state)
{
	/* Check channel is valid*/
	if (channel_index >= CHANNEL_1 && channel_index < CHANNEL_NOOF)
	{
		/* Set the new state of the channel*/
		scent_channel_t* channel = &channel_manager.channel[channel_index];
		return scent_channel_set_state(channel, state);
	} 
	else
	{
		/* Channel was not valid */
		return CM_ERROR_GENERAL;
	}
}

/* Set a channels name */
ow_errors_t channel_manager_set_channel_name(channel_manager_index_t channel_index, char *name)
{
	/* Check channel is valid*/
	if (channel_index >= CHANNEL_1 && channel_index < CHANNEL_NOOF)
	{
		/* Set the new state of the channel*/
		scent_channel_t* channel = &channel_manager.channel[channel_index];
		return scent_channel_set_name(channel, name);
	}
	else
	{
		/* Channel was not valid */
		return CM_ERROR_GENERAL;
	}
}
