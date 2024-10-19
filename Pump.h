#ifndef PUMP_H
#define PUMP_H

#ifdef __cplusplus
extern"C"
{
#endif

#include "stdint.h"
#include "stdbool.h"
#include "ow_errors.h"

#define PUMP_NAME_LENGTH	25
/* Pump structure, contains all data associated with a single pump 
 * Allows expansion of hardware to include more pumps if necessary */
typedef struct pump
{
	uint8_t index;
	uint8_t gpio;
	char name[PUMP_NAME_LENGTH];
	uint32_t actuation_count;
}pump_t;

/* Public pump functionality */
ow_errors_t pump_init(pump_t* this, uint8_t index, const char* name, const uint8_t gpio);
ow_errors_t pump_set_state(pump_t* this, bool state);
ow_errors_t pump_set_name(pump_t* this, const char* name);

#ifdef __cplusplus
}
#endif

#endif