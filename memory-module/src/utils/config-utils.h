#ifndef CONFIG_UTILS_H_
#define CONFIG_UTILS_H_

	#include <stdlib.h>
	#include <commons/config.h>
	#include <commons/string.h>
	#include <string.h>
	#include <commons/log.h>
	#include "logger-utils.h"
	#include "../structs/config.h"

	typedef struct {
		char *PORT;
		char *IP;
		int MEMORY_SIZE;
		int SEGMENT_ZERO_SIZE;
		int SEGMENT_COUNT;
		int MEMORY_DELAY;
		int COMPACTION_DELAY;
		char *ASSIGNMENT_ALGORITHM;
		t_log_level LOG_LEVEL;
	} t_memory_config;

	void create_memory_config(char *fileName);
	t_memory_config* get_memory_config();
	t_assignment_algorithm get_assignment_algorithm();
	void log_config(t_memory_config *config);

#endif
