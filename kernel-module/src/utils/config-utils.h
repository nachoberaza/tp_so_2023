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
	char *IP_FILESYSTEM;
	char *PORT_FILESYSTEM;
	char *IP_CPU;
	char *PORT_CPU;
	char *PORT_LISTEN;
	char *PLANNING_ALGORITHM;
	int INITIAL_ESTIMATE;
	int HRRN_ALFA;
	int MAX_MULTIPROGRAMMING_LEVEL;
	t_log_level LOG_LEVEL;
} t_kernel_config;

t_kernel_config* create_kernel_config(char *moduleName);
void log_config(t_kernel_config *config);

#endif
