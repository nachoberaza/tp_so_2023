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

	int get_physical_address(t_log_grouping* logger, t_execution_context* context, char* logicalAddress, int segmentMaxSize);

#endif
