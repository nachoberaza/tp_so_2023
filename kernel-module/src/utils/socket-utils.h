#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
	#include "config-utils.h"
	#include "pcb-utils.h"
	#include "long-term-scheduler-utils.h"

	typedef struct {
		int cpu;
		int fileSystem;
		int memory;
	} t_kernel_connections;

	void start_connections(t_kernel_config* env);
	t_kernel_connections* get_kernel_connections();
	int get_cpu_connection();
	int get_file_system_connection();
	int get_memory_connection();
	int handle_console_handshake(int clientSocketId);
	void listen_consoles(int serverSocket);


#endif
