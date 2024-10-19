#ifndef OW_ERRORS_H
#define OW_ERRORS_H

#ifdef __cplusplus
extern"C"
{
#endif

/* System error / return values */
typedef enum
{
	/* Generic OK return code */
	ERROR_OK,

	/* Pump Errors */
	PUMP_ERROR_INIT_FAILED,
	PUMP_ERROR_GENERAL,

	/* Pump manager errors */
	PM_ERROR_INIT_FAILED,
	PM_ERROR_GENERAL,

	/* Scent channel errors*/
	SC_ERROR_INIT_FAILED,
	SC_ERROR_GENERAL,

	/* Channel manager errors */
	CM_ERROR_OK,
	CM_ERROR_INIT_FAILED,
	CM_ERROR_GENERAL,

	/* Run cycle errors */
	RC_ERROR_INIT_FAILED,
	RC_ERROR_GENERAL,

	/* Serial parser errors */
	PARSER_ERROR_INIT_FAILED,
	PARSER_ERROR_GENERAL,

}ow_errors_t;

#ifdef __cplusplus
}
#endif

#endif