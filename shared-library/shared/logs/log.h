#ifndef SHARED_LOGS_H_
#define SHARED_LOGS_H_

	#include <commons/log.h>
	#include <commons/string.h>

	typedef enum {
		LOG_TARGET_ALL,
		LOG_TARGET_MAIN,
		LOG_TARGET_INTERNAL,
	}t_log_target;

	typedef struct {
		t_log* main;
		t_log* internal;
	}t_log_grouping;

	t_log* start_logger(char *module, t_log_level logLevel, int writeToConsole);

	void write_log(t_log* log, t_log_level logLevel, char* message);
	void write_log_grouping(t_log_grouping* log, t_log_target logTarget, t_log_level logLevel, char* message);

#endif
