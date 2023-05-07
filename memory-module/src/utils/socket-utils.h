#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_
	#include <stdlib.h>
	#include <pthread.h>
	#include "config-utils.h"
	#include "logger-utils.h"
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>

	int handle_handshake(void* clientSocketId);
	char* determinate_module(int module);
	void accept_module(void* socketId, int module);
	void* listen_kernel_connection(void *clientSocket);
	int handle_kernel_connection(void *clientSocket);

#endif
