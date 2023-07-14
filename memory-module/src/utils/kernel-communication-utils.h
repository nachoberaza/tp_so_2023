#ifndef UTILS_KERNEL_COMMUNICATION_UTILS_H_
#define UTILS_KERNEL_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include<unistd.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <commons/collections/list.h>
	#include <shared/logs/log.h>
	#include <shared/structs/memory.h>
	#include <shared/structs/socket.h>
	#include <shared/structs/execution-context.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/memory/memory.h>
	#include <shared/execution-context/execution-context.h>
	#include "config-utils.h"
	#include "logger-utils.h"
	#include "memory-utils.h"


	void listen_kernel_connection(int clientSocketId);
	void execute_memory_delete_segment(t_memory_data* data, int clientSocketId);
	void execute_memory_create_segment(t_memory_data* data, int clientSocketId);
	void execute_memory_compress_segment_table(int clientSocketId);
	void execute_memory_delete_segment_table(int clientSocketId);
	void execute_memory_create_segment_table(int clientSocketId);
	void execute_memory_kernel_instruction(int clientSocketId);
	int compare_base_segment_row(void* a, void* b);
#endif
