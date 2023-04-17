#include "shared-logs.h"

t_log* start_logger(char *module, t_log_level logLevel, int writeToConsole) {
	char *fileName = string_from_format("%s.log", module);

	return log_create(fileName, module, writeToConsole, logLevel);
}

void write_log(t_log* log, t_log_level logLevel, char* message) {
	switch(logLevel){
		case LOG_LEVEL_TRACE:
			log_trace(log, message);
				break;
		case LOG_LEVEL_DEBUG:
			log_debug(log, message);
				break;
		case LOG_LEVEL_INFO:
			log_info(log, message);
				break;
		case LOG_LEVEL_WARNING:
			log_warning(log, message);
				break;
		case LOG_LEVEL_ERROR:
			log_error(log, message);
				break;
		default:
			log_error(log, "Log level no se corresponde a un valor existente");
			log_error(log, message);
			break;
	}
}

void write_log_grouping(t_log_grouping* logGroup, t_log_target logTarget,
			t_log_level logLevel, char* message) {
	switch(logTarget){
			case LOG_TARGET_ALL:
				write_log(logGroup->main, logLevel, message);
				write_log(logGroup->internal, logLevel, message);
					break;
			case LOG_TARGET_MAIN:
				write_log(logGroup->main, logLevel, message);
					break;
			case LOG_TARGET_INTERNAL:
				write_log(logGroup->internal, logLevel, message);
					break;
			default:
				write_log(logGroup->internal, logLevel, "Log target no se corresponde a un valor existente");
				write_log(logGroup->main, logLevel, message);
				write_log(logGroup->internal, logLevel, message);
				break;
		}
}
