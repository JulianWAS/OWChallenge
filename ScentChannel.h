#ifndef SCENT_CHANNEL_H
#define SCENT_CHANNEL_H

#ifdef __cplusplus
extern"C"
{
#endif

#include "stdint.h"
#include "stdbool.h"
#include "ow_errors.h"

#define SCENT_CHANNEL_NAME_LENGTH	25

/* Define object structure for a refill cartridge 
 * The refill cartridge should be it's own 'object file' but I ran out of time */
typedef struct 
{
	uint32_t level;
	uint32_t uuid;
	uint32_t actuation_count;
}refill_cartridge_t;

/* Define object structure for a single scent channel */
typedef struct 
{
	uint8_t index;
	uint8_t gpio;
	char name[SCENT_CHANNEL_NAME_LENGTH];

	bool state;
	uint32_t actuation_count;
	refill_cartridge_t cartridge;
}scent_channel_t;


/* Scent channel public functionality */
ow_errors_t scent_channel_init(scent_channel_t* this, uint8_t index, const char* name, const uint8_t gpio);
ow_errors_t scent_channel_set_state(scent_channel_t* this, bool open);
ow_errors_t scent_channel_set_name(scent_channel_t* this, const char* name);

#ifdef __cplusplus
}
#endif


#endif

