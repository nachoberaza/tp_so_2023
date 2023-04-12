#ifndef CONFIG_UTILS_H_
#define CONFIG_UTILS_H_

#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "logger-utils.h"

typedef struct {
	char *PORT;
	char *IP;
	int MEMORY_SIZE;
	int SHARED_MEMORY_SEGMENT;
	int SEGMENT_COUNT;
	int MEMORY_DELAY;
	int COMPACTION_DELAY;
	char *ASSIGNMENT_ALGORITHM;
	t_log_level LOG_LEVEL;
} t_memory_config;

t_memory_config* create_memory_config(char *moduleName);
void log_config(t_memory_config *config);

#endif
