#ifndef UTILS_MEMORY_UTILS_H_
#define UTILS_MEMORY_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/structs/memory.h>
	#include "config-utils.h"
	#include "logger-utils.h"

	void create_memory_structures();
	t_list* create_segment_table();
	void* get_memory();

#endif
