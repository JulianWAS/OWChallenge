#ifndef CHANNEL_MANAGER_H
#define CHANNEL_MANAGER_H

#ifdef __cplusplus
extern"C"
{
#endif

#include "stdint.h"
#include"ScentChannel.h"
#include "ow_errors.h"

#define MAX_SCENT_CHANNELS	6

/* Channel enumerations for clarity */
typedef enum
{
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
	CHANNEL_5,
	CHANNEL_6,
	CHANNEL_NOOF,
}channel_manager_index_t;

/* Manager structure to hold all data concerning the manager */
typedef struct
{
	scent_channel_t channel[MAX_SCENT_CHANNELS];
}channel_manager_t;

/* Public manager functionality */
ow_errors_t channel_manager_init(void);
ow_errors_t channel_manager_set_channel_state(channel_manager_index_t channel_index, bool state);
ow_errors_t channel_manager_set_channel_name(channel_manager_index_t channel_index, char* name);

#ifdef __cplusplus
}
#endif

#endif
