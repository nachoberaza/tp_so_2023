#ifndef UTILS_CPU_COMMUNICATION_UTILS_H_
#define UTILS_CPU_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <commons/string.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/structs/socket.h>
    #include "config-utils.h"
	#include "logger-utils.h"
	#include "memory-utils.h"

	void listen_cpu_connection(int clientSocketId);
	void execute_memory_mov_in(t_instruction* instruction, int clientSocketId);
	void execute_memory_mov_out(t_instruction* instruction, int clientSocketId);

#endif
