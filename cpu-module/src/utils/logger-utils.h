#ifndef LOGGER_UTILS_H_
#define LOGGER_UTILS_H_

	#include <commons/log.h>
	#include <stdlib.h>
	#include "log/shared-logs.h"

	void init_logger(char* moduleName, t_log_level logLevel);
	void write_to_log(t_log_target target, t_log_level logLevel, char* message);
	t_log_grouping* getLogger();

	//Only reason for this is so it can be passed as a parameter with only a char*
	void write_info_to_all_logs(char* message);

#endif
