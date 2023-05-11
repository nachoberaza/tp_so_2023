#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_
	#include <stdlib.h>
	#include <pthread.h>
	#include "config-utils.h"
	#include "logger-utils.h"
    #include "structs.h"
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>

	void handle_handshake(int clientSocketId,pthread_t *modules_thread_id);
	char *module_as_string(module_handshakes module);
	void add_pthread_id(pthread_t *module_thread, pthread_t *modules_thread_id);
	void accept_module(int clientSocketId, int module);
	void* listen_kernel_connection(void *clientSocket);
	pthread_t handle_kernel_connection(int *clientSocketId);

#endif
