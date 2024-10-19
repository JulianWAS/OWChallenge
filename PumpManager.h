#ifndef PUMP_MANAGER_H
#define PUMP_MANAGER_H

#ifdef __cplusplus
extern"C"
{
#endif

#include "stdint.h"
#include "stdbool.h"
#include "Pump.h"
#include "ow_errors.h"

#define MAX_PUMPS	1

/* Enumeration of allowed pumps */
typedef enum
{
	PUMP_1,
	PUMP_NOOF
}pump_manager_index_t;

/* Pump management data. Although this level of complexity is not strictly necessary it
 * 1. Keeps a common object / manager control scheme 
 * 2. Allows the firmware to adapt to potential hardware changes with minimal impact
 */
typedef struct pump_manager
{
	pump_t pump[MAX_PUMPS];

}pump_manager_t;

/* Public pump management functionality */
ow_errors_t pump_manager_init(void);
ow_errors_t pump_manager_set_all_state(bool state);
ow_errors_t pump_manager_set_index_state(pump_manager_index_t pump_index, bool state);
ow_errors_t pump_manager_set_name(pump_manager_index_t pump_index, char* name);

#ifdef __cplusplus
}
#endif

#endif
