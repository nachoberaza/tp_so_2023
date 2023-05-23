#include "logger-utils.h"

t_log_grouping* logger;

void init_logger(char* moduleName, t_log_level logLevel) {
	char *internalLogName = string_from_format("%s.extra", moduleName);
	logger = malloc(sizeof(t_log_grouping));

	//Only write to console in the internal logger to avoid duplication
	logger->main = start_logger(moduleName, logLevel, 0);
	logger->internal = start_logger(internalLogName, logLevel, 1);
}

t_log_grouping* get_logger(){
	return logger;
}

void write_to_log(t_log_target target, t_log_level logLevel, char* message) {
	write_log_grouping(logger, target, logLevel, message);
}
