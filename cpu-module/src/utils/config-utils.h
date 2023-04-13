#ifndef CONFIG_UTILS_H_
#define CONFIG_UTILS_H_

#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "logger-utils.h"

typedef struct {
	char *IP;
	char *PORT;
	char *IP_MEMORY;
	char *PORT_MEMORY;
	int INSTRUCTION_DELAY;
	int SEGMENT_MAX_SIZE;
	t_log_level LOG_LEVEL;
} t_cpu_config;

t_cpu_config* create_cpu_config(char *moduleName);
void log_config(t_cpu_config *config);

#endif
