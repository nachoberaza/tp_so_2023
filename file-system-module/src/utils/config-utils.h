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
		char *PATH_SUPERBLOQUE;
		char *PATH_BITMAP;
		char *PATH_BLOQUES;
		char *PATH_FCB;
		int BLOCK_ACCESS_DELAY;
		t_log_level LOG_LEVEL;
	} t_file_system_config;

	void init_file_system_config(char *moduleName);
	t_file_system_config* get_file_system_config();
	void log_config(t_file_system_config *config);

#endif




