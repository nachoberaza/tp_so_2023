#ifndef UTILS_MEMORY_UTILS_H_
#define UTILS_MEMORY_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/structs/memory.h>
	#include <shared/structs/socket.h>
	#include "config-utils.h"
	#include "kernel-communication-utils.h"
	#include "logger-utils.h"
	#include "../structs/config.h"

	void create_memory_structures();
	t_list* create_segment_table();
	void* get_memory();
	operation_result delete_segment_if_exists(int segmentId);
	operation_result add_to_memory(t_segment_row* segment);
	operation_result add_segment_best_algorithm(t_segment_row* segment);
	operation_result add_segment_first_algorithm(t_segment_row* segment);
	operation_result add_segment_worst_algorithm(t_segment_row* segment);
#endif
