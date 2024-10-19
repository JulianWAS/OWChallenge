#ifndef MOCK_H
#define MOCK_H

#ifdef __cplusplus
extern"C"
{
#endif

/* File to mock various common RTOS and SDK style functiuons to allow compilation*/

/* Mock GPIO functions */
#define GPIO_SET(x,y) true
#define HIGH true
#define LOW false

/* Mock RTOS timer, task and panic functions */
#define RTOS_INIT()	0
#define START_TIMER(x, y, z) true
#define PANIC()
#define START_TASK(x,y) true

/* Serial read mocked returns*/
typedef enum
{
	SERIAL_NO_DATA,
	SERIAL_FULL_DATA,
	SERIAL_PARTIAL_DATA,
	SERIAL_ERROR,
	SERIAL_NOOF
}serial_responses_t;

#ifdef __cplusplus
}
#endif

#endif
