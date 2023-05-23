#ifndef LOGGER_UTILS_H_
#define LOGGER_UTILS_H_

	#include <stdlib.h>
	#include <commons/log.h>
	#include <shared/logs/log.h>
	
	void init_logger(char* moduleName, t_log_level logLevel);
	t_log_grouping* get_logger();
	void write_to_log(t_log_target target, t_log_level logLevel, char* message);

#endif
