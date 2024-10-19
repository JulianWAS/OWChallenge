#include "stdio.h"
#include "ScentChannel.h"
#include "mock.h"
#include "string.h"

#define REFILL_MIINIMUM_OPERATION_LEVEL	0x00010000

/* Initialise a single scent channel 
 * default values are passed in but if initialisation has already happened, this data
 * needs to be saved in non volatile memory to preseve counts etc.
 */
ow_errors_t scent_channel_init(scent_channel_t* this, uint8_t index, const char* name, const uint8_t gpio)
{
	/* Check flash for saved parameters */
	scent_channel_t *saved_data = NULL; // replace with function to get saved data

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
		scent_channel_set_name(this, name);

		/* As this is a first time init we need to setiup the refill */
		uint32_t refill_uuid = 0x12341234; // function to read / get the refill uuid
		if (refill_uuid != 0) 
		{
			this->cartridge.uuid = refill_uuid;
			this->cartridge.actuation_count = 0;
			this->cartridge.level = 0;

			/* Save initialized channel here */
		}
		else 
		{
			/* can't find a refill */
			return SC_ERROR_INIT_FAILED;
		}
	}

	return ERROR_OK;
}


/* This assumes that channels do not need to be opened simultaneaously
 * If they do then, possible hardware discussion to enable moving all to the same gpio port
 * with at least 6 pins free and using bit masks to set them all at once. In
 * this case there would need to be an interface change. */
ow_errors_t scent_channel_set_state(scent_channel_t* this, bool open)
{
	/* Open or close the channel?*/
	if (open) 
	{
		/* Check for refill level - other checks may be done */
		if (this->cartridge.level > REFILL_MIINIMUM_OPERATION_LEVEL) 
		{
			/* Open the channel */
			if (GPIO_SET(this->gpio, HIGH) == true)
			{
				/* Simple inc here but handle wrap at some point*/
				this->actuation_count++;
				this->cartridge.actuation_count++;

				/* Save updated info here */
			} 
			else
			{
				/* actuation failed */
				return SC_ERROR_GENERAL;
			}
		}
		else 
		{
			/* refill level is too low return error*/
			return SC_ERROR_GENERAL;
		}
	}
	else 
	{
		/* close the channel */
		if (GPIO_SET(this->gpio, LOW) == true)
		{
			uint32_t new_level = 0; // Function to calculate new level of refill
			this->cartridge.level = new_level;
		}
		else
		{
			/* actuation failed */
			return SC_ERROR_GENERAL;
		}
	}

	return ERROR_OK;
}

/* function is very similar to pump version - common them at some point */
ow_errors_t scent_channel_set_name(scent_channel_t* this, const char* name)
{
	if (!strncpy_s(this->name, SCENT_CHANNEL_NAME_LENGTH, name, SCENT_CHANNEL_NAME_LENGTH))
	{
		/* Ensure string is null terminated */
		this->name[SCENT_CHANNEL_NAME_LENGTH - 1] = 0;
		
		/* Save updated info here */
		return ERROR_OK;
	}

	return SC_ERROR_GENERAL;
}
