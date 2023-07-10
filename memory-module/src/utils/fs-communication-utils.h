#ifndef UTILS_FS_COMMUNICATION_UTILS_H_
#define UTILS_FS_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <commons/string.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/memory/memory.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/structs/socket.h>
    #include "config-utils.h"
	#include "logger-utils.h"
	#include "memory-utils.h"

	void listen_filesystem_connection(int clientSocketId);
	void execute_memory_f_read(t_memory_data* data, int clientSocketId, char* origin);
	void execute_memory_f_write(t_memory_data* data, int clientSocketId,char* origin);

#endif
