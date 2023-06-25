#ifndef UTILS_MEMORY_COMMUNICATION_UTILS_H_
#define UTILS_MEMORY_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <math.h>
	#include <stdlib.h>
	#include <unistd.h>// For sleep
	#include <commons/log.h>
	#include <commons/string.h>
	#include <commons/collections/list.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/structs/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/memory/memory.h>
	#include "config-utils.h"
	#include "logger-utils.h"
	#include "commands-utils.h"
	#include "socket-utils.h"

	void send_mov_in_to_memory(t_execution_context* context, char* reg, int physicalAddress);
	void send_mov_out_to_memory(t_execution_context* context, char* reg, int physicalAddress);
	void handle_mov_in_memory_response(t_execution_context* context);

#endif
