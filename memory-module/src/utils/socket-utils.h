#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_
	#include <stdlib.h>
	#include <pthread.h>
    #include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/structs/memory.h>
	#include "config-utils.h"
	#include "logger-utils.h"
	#include "memory-utils.h"
	#include "kernel-communication-utils.h"

	//Only for sleep, must be removed eventually
	#include <unistd.h>

	#include "../structs/modules.h"

	t_modules_thread_id *get_modules_thread_id();

	void handle_handshake(int clientSocketId);
	void accept_module(int clientSocketId, int module);
	void await_modules(int serverSocketId);

	void listen_kernel_connection(int clientSocketId);
	void listen_cpu_connection(int clientSocketId);
	void listen_filesystem_connection(int clientSocketId);

	void handle_kernel_connection(int clientSocketId);
	void handle_cpu_connection(int clientSocketId);
	void handle_filesystem_connection(int clientSocketId);

#endif
