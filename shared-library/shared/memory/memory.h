#ifndef SHARED_MEMORY_MEMORY_H_
#define SHARED_MEMORY_MEMORY_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <math.h>
	#include <commons/collections/list.h>
	#include "../logs/log.h"
	#include "../structs/memory.h"
	#include "../structs/execution-context.h"
	#include "../execution-context/execution-context.h"
	#include "../serialization/serialization.h"

	typedef struct {
		int pid;
		t_instruction* instruction;
	} t_memory_data;


	int get_physical_address(t_log_grouping* logger, t_execution_context* context, int size, char* logicalAddress, int segmentMaxSize);
	t_memory_data* decode_memory_data(t_log_grouping* logger,int clientSocketId);
	void fill_buffer_with_memory_data(t_memory_data* memoryData, t_package* pkg);
	void log_memory_data(t_memory_data* data,t_log_grouping* logger,t_log_level logLevel);

#endif
