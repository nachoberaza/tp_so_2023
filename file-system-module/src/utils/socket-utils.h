#ifndef UTILS_SOCKET_UTILS_H_
#define UTILS_SOCKET_UTILS_H_
	#include <stdio.h>
	#include <stdlib.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
    #include "config-utils.h"
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/structs/socket.h>
	#include "../utils/logger-utils.h"

	void start_memory_connection(t_file_system_config* fileSystemEnv);
	t_file_system_config* get_memory_connection();
	void await_kernel(int serverSocketId);
	void handle_handshake(int clientSocketId);
	void listen_kernel_connection(int clientSocketId);
	operation_result execute_fs_f_truncate(t_instruction* instruction);
	operation_result execute_fs_f_close(t_instruction* instruction);
	operation_result execute_fs_f_write(t_instruction* instruction);
	operation_result execute_fs_f_open(t_instruction* instruction);
	operation_result execute_fs_f_seek(t_instruction* instruction);
	operation_result execute_fs_f_read(t_instruction* instruction);
#endif
