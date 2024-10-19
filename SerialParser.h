#ifndef SERIAL_PARSER_H
#define SERIAL_PARSER_H

#ifdef __cplusplus
extern"C"
{
#endif

#include "stdint.h"
#include "mock.h"
#include "ow_errors.h"

#define MAX_TLV_PAYLOAD	50
#define PACKET_TLV_OVERHEAD 2

/* Definition of the tags or commands available to the serial interface. */
typedef enum
{
	/* Operation Commands*/
	TAG_RESET,
	TAG_SET_CHANNEL,
	TAG_START_CYCLE,

	/* Configuration commands */
	TAG_CONFIG_SET_CHANNEL_NAME,
	TAG_CONFIG_SET_PUMP_NAME,

	/* Other command groups that couuld be added, further config, debug, logging, etc*/

	TAG_NOOF
}parser_tag_t;

/* Very simple TLV structure */
typedef struct
{
	uint8_t tag;
	uint8_t	length;
	/* We may also want some sort of crc or checksum as part of the payload to validate the packet */
	uint8_t value[MAX_TLV_PAYLOAD];
}parser_packet_t;

/* Command callback definition */
typedef ow_errors_t(*serial_parser_function_cb_t)(parser_packet_t* packet);

/* Parser command definition structure */
typedef struct
{
	parser_tag_t tag;
	uint8_t length;
	serial_parser_function_cb_t cb;
}parser_function_definition_t;

/* Parser public functionality */
ow_errors_t serial_parser_start(void);

#ifdef __cplusplus
}
#endif


#endif
