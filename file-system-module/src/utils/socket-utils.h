#ifndef UTILS_SOCKET_UTILS_H_
#define UTILS_SOCKET_UTILS_H_
	#include <stdio.h>
	#include <stdlib.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/sockets/server-socket.h>
    #include "config-utils.h"

void start_memory_connection(t_file_system_config* fileSystemEnv);
t_file_system_config* get_memory_connection();


#endif
