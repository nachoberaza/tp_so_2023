#ifndef MEMORY_MODULE_H_
#define MEMORY_MODULE_H_

#define MODULE_NAME "memory"

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <pthread.h>
	#include "utils/config-utils.h"
	#include "utils/logger-utils.h"

	void* handleConnection(void *clientSocket);
	int handle_handshake(int socket);

#endif
