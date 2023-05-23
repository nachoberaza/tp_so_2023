#ifndef UTILS_COMMANDS_UTILS_H_
#define UTILS_COMMANDS_UTILS_H_

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

	int execute_set(t_execution_context* context, t_instruction* currentInstruction);
	int execute_yield(t_execution_context* context);
	int execute_exit(t_execution_context* context);

#endif
