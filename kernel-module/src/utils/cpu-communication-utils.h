
#ifndef CPU_COMMUNICATION_UTILS_H_
#define CPU_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/serialization/serialization.h>
	#include "config-utils.h"
	#include "socket-utils.h"
	#include "logger-utils.h"
	#include "pcb-utils.h"
	#include "thread-utils.h"
	#include "../structs/pcb.h"
	// For sleep
	#include <unistd.h>

	void handle_processes();
	void execute_process();
	void send_context_to_cpu(t_execution_context *commands);
	t_execution_context* listen_cpu_response();
	void handle_cpu_response(t_pcb* pcb);

#endif
