#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_
	#include <stdlib.h>
	#include <pthread.h>
	#include "config-utils.h"
	#include "logger-utils.h"
    #include "structs.h"
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>

	void handle_handshake(int clientSocketId);
	char *module_as_string(module_handshakes module);
	void accept_module(int clientSocketId, int module);
	void listen_kernel_connection(int clientSocketId);
	void listen_cpu_connection(int clientSocketId);
	void listen_filesystem_connection(int clientSocketId);
	void handle_kernel_connection(int clientSocketId);
	void handle_cpu_connection(int clientSocketId);
	void handle_filesystem_connection(int clientSocketId);
	t_modules_thread_id *get_modules_thread_id();
	void init_modules();

#endif
