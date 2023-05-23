#ifndef UTILS_EXECUTION_CONTEXT_UTILS_H_
#define UTILS_EXECUTION_CONTEXT_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include "config-utils.h"
	#include "logger-utils.h"
	#include "commands-utils.h"

	void execute_execution_context(t_execution_context* context);
	void return_execution_context(t_execution_context* context, int clientSocketId);

#endif
