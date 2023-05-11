#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
	#include "config-utils.h"
	#include "pcb-utils.h"

	typedef struct {
		int cpu;
		int fileSystem;
		int memory;
	} t_kernel_connections;

	t_kernel_connections* start_connections(t_kernel_config* env);
	int handle_console_handshake(int clientSocketId);
	void listen_consoles(int serverSocket);

#endif
