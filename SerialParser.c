#include <stdio.h>
#include <string.h>
#include "ChannelManager.h"
#include "PumpManager.h"
#include "SerialParser.h"
#include "RunCycle.h"
#include "mock.h"

#define MAX_SERIAL_BUFFER_LENGTH	1024

/* Task */
static ow_errors_t parser_task(void);

/* Statics */
static ow_errors_t serial_parser_reset(void);
static serial_responses_t serial_parser_read(uint8_t* buffer, uint32_t buffer_size, uint32_t* length);
static const parser_function_definition_t* parser_find_definition(parser_packet_t* packet);
static ow_errors_t parser_verify_packet(const parser_function_definition_t* definition, parser_packet_t* packet);
static ow_errors_t serial_parser_parse(void);
static ow_errors_t serial_parser_copy(uint8_t* buffer, uint32_t length);


/* Callbacks */
static ow_errors_t parser_reset_device(parser_packet_t* packet);
static ow_errors_t parser_set_channels(parser_packet_t* packet);
static ow_errors_t parser_start_cycle(parser_packet_t* packet);
static ow_errors_t parser_set_channel_name(parser_packet_t* packet);
static ow_errors_t parser_set_pump_name(parser_packet_t* packet);

/* Definitions */
const parser_function_definition_t parser_tag_definitions[] =
{
	{TAG_RESET, 0, parser_reset_device},
	{TAG_SET_CHANNEL, 2, parser_set_channels},
	{TAG_START_CYCLE, 0, parser_start_cycle},

	{TAG_CONFIG_SET_CHANNEL_NAME, MAX_TLV_PAYLOAD, parser_set_channel_name},
	{TAG_CONFIG_SET_PUMP_NAME, MAX_TLV_PAYLOAD, parser_set_pump_name},
	{TAG_NOOF, 0, NULL}
};

/* Just using a simple buffer for copying into and parsing out of - actual solution would be to parse directly out of a 
   ring buffer that was written into probably from a dma driven UART buffer*/
uint8_t parse_buffer[MAX_SERIAL_BUFFER_LENGTH];
uint32_t parse_buffer_write_pos;
uint32_t parse_buffer_read_pos;


/* Initialsie the uart and start the task here */
ow_errors_t serial_parser_start(void)
{
	/* Start the parser task here */
	if (START_TASK(parser_task, NULL))
	{
		return ERROR_OK;
	}

	return PARSER_ERROR_INIT_FAILED;
}

static ow_errors_t parser_task(void)
{
	serial_parser_reset();
	while (1)
	{
		uint8_t buffer[MAX_SERIAL_BUFFER_LENGTH];
		uint32_t read_length = 0;

		serial_responses_t response = serial_parser_read(buffer, MAX_SERIAL_BUFFER_LENGTH, &read_length);
		switch (response)
		{
		case SERIAL_NO_DATA:
			break;

		case SERIAL_FULL_DATA:
			if (serial_parser_copy(buffer, read_length) == ERROR_OK)
			{
				if (serial_parser_parse())
				{
					/* An error occurred this may be dealt with by the 'fallthrough reset below however
					 * certain errors may need to be dealt with specifically. 
					 * For example if the 'TAG_RESET' command fails then we may wish to flag this or allow
					 * a breakpoint hook for debugging etc. */
				}
			}
			serial_parser_reset();
			break;

		case SERIAL_PARTIAL_DATA:
			if (serial_parser_copy(buffer, read_length) != ERROR_OK)
			{
				serial_parser_reset();
			}
			break;
	
		default:
		case SERIAL_ERROR:
			serial_parser_reset();
			break;
		}

		/* Wait or yeild here depending on RTOS 
		 * this can effect power consumption heavily 
		 * so possibly have some sort of serial preamble that acts a wakeup or similar (wakeup gpio or timer) if needed */
	}
}

static serial_responses_t serial_parser_read(uint8_t *buffer, uint32_t buffer_size, uint32_t* length)
{
	/* Reads data from the serial port into a buuffer - assumes chunks of data
		not single characters */
	/* returns data in given buffer, a length of read data and whether data is complete partial or error */
	*length = 50; // will be actual length of data read 
	return SERIAL_FULL_DATA;
}

static const parser_function_definition_t* parser_find_definition(parser_packet_t* packet)
{
	for (uint8_t i = 0; i < TAG_NOOF; i++)
	{
		if (packet->tag == parser_tag_definitions[i].tag)
		{
			return &parser_tag_definitions[i];
		}
	}

	return NULL;
}

static ow_errors_t parser_verify_packet(const parser_function_definition_t* definition, parser_packet_t* packet)
{
	/* Simple max length check - can add trailing crc to be almost certain*/
	if (packet->length <= definition->length)
	{
		return ERROR_OK;
	}

	return PARSER_ERROR_GENERAL;
}

static ow_errors_t serial_parser_parse(void)
{
	parser_packet_t* packet = (parser_packet_t*) & parse_buffer[parse_buffer_read_pos];

	const parser_function_definition_t* definition = parser_find_definition(packet);
	if (definition != NULL)
	{
		if (parser_verify_packet(definition, packet) == ERROR_OK)
		{
			if (definition->cb(packet) != ERROR_OK)
			{
				return PARSER_ERROR_GENERAL;
			}
		}
	}

	parse_buffer_read_pos += packet->length + PACKET_TLV_OVERHEAD;

	return ERROR_OK;
}

static ow_errors_t serial_parser_copy(uint8_t* buffer, uint32_t length)
{
	/* Simple overflow check */
	if ((parse_buffer_write_pos = length) < MAX_SERIAL_BUFFER_LENGTH)
	{
		/* Copy data into the parse buffer */
		memcpy(&parse_buffer[parse_buffer_write_pos], buffer, length);
		parse_buffer_write_pos += length;
		return ERROR_OK;
	} 
	else
	{
		return PARSER_ERROR_GENERAL;
	}

}

static ow_errors_t serial_parser_reset(void)
{
	parse_buffer_write_pos = 0;
	parse_buffer_read_pos = 0;
	return ERROR_OK;
}

static ow_errors_t parser_reset_device(parser_packet_t* packet)
{
	/* Hard reset the device 
		additional parameters could clear flash / data memory
		put into booloader mode
		force reflash of previous stored image
		put into debug mode
		etc.
		*/

	/* This function will reset the entire device so if it gets here there is a problem */
	return PARSER_ERROR_GENERAL;
}

static ow_errors_t parser_set_channels(parser_packet_t* packet)
{
	typedef struct
	{
		uint8_t channel;
		uint8_t value;
	}set_channel_payload;

	set_channel_payload* payload = (set_channel_payload *)packet->value;
	if (channel_manager_set_channel_state(payload->channel, payload->value))
	{
		return PARSER_ERROR_GENERAL;
	}
	
	return ERROR_OK;
}

static ow_errors_t parser_start_cycle(parser_packet_t* packet)
{
	run_cycle_start();
	return ERROR_OK;
}

static ow_errors_t parser_set_channel_name(parser_packet_t* packet)
{
	typedef struct
	{
		uint8_t channel;
		char name[];
	}set_channel_name_payload;
	
	set_channel_name_payload* payload = (set_channel_name_payload* )packet->value;
	channel_manager_set_channel_name(payload->channel, payload->name);
	return ERROR_OK;
}

static ow_errors_t parser_set_pump_name(parser_packet_t* packet)
{
	typedef struct
	{
		uint8_t pump;
		char name[];
	}set_puump_name_payload;

	set_puump_name_payload* payload = (set_puump_name_payload *)packet->value;
	pump_manager_set_name(payload->pump, payload->name);
	return ERROR_OK;
}