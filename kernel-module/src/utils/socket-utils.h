#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_

	#include <shared/sockets/client-socket.h>
	#include "config-utils.h"

	typedef struct {
		int cpu;
		int fileSystem;
		int memory;
	} t_kernel_connections;

	t_kernel_connections* start_connections(t_kernel_config* env);

#endif
