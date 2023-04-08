#ifndef CONFIG_UTILS_H_
#define CONFIG_UTILS_H_

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "shared-logs.h"

typedef struct {
	char *IP_KERNEL;
	char *PORT_KERNEL;
	t_log_level LOG_LEVEL;
} t_console_config;

t_console_config* create_console_config(char *moduleName);
void log_config(t_console_config *config);

#endif
