#ifndef UTILS_SOCKET_UTILS_H_
#define UTILS_SOCKET_UTILS_H_
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
    #include "config-utils.h"
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/structs/socket.h>
	#include "../utils/logger-utils.h"
	#include "../utils/file-utils.h"

	void start_memory_connection(t_file_system_config* fileSystemEnv);
	int get_memory_connection();
	void await_kernel(int serverSocketId);
	void handle_handshake(int clientSocketId);
	t_instruction* decode_instruction(int clientSocketId);
	void listen_kernel_connection(int clientSocketId);

#endif
