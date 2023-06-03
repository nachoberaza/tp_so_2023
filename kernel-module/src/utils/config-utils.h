#ifndef CONFIG_UTILS_H_
#define CONFIG_UTILS_H_

#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "logger-utils.h"
#include "../structs/config.h"

typedef struct {
	char *IP;
	char *PORT;
	char *IP_MEMORY;
	char *PORT_MEMORY;
	char *IP_FILESYSTEM;
	char *PORT_FILESYSTEM;
	char *IP_CPU;
	char *PORT_CPU;
	char *PLANNING_ALGORITHM;
	double INITIAL_ESTIMATE;
	double HRRN_ALFA;
	char **RESOURCES;
	char **RESOURCES_INSTANCES;
	int MAX_MULTIPROGRAMMING_LEVEL;
	t_log_level LOG_LEVEL;
} t_kernel_config;

void init_kernel_config(char *moduleName);
t_kernel_config* get_kernel_config();
planning_algorithm get_planning_algorithm();
void log_config(t_kernel_config *config);

#endif
