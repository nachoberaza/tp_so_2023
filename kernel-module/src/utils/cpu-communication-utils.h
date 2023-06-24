#ifndef CPU_COMMUNICATION_UTILS_H_
#define CPU_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <time.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/structs/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/execution-context/execution-context.h>
	#include "config-utils.h"
	#include "socket-utils.h"
	#include "logger-utils.h"
	#include "pcb-utils.h"
	#include "thread-utils.h"
	#include "resource-utils.h"
	#include "../structs/pcb.h"
	#include "../structs/resource.h"
	#include "../structs/config.h"
	#include "semaphore-utils.h"
	#include "short-term-scheduler-utils.h"
	#include "memory-communication-utils.h"
	// For sleep
	#include <unistd.h>

	typedef struct{
		int timeIO;
		t_pcb* pcb;
	}t_io_thread_args;

	void handle_processes();
	void execute_process();
	void send_context_to_cpu(t_execution_context *commands);
	t_execution_context* listen_cpu_response();
	void handle_cpu_response(t_pcb* pcb);
	void execute_kernel_wait(t_pcb* pcb);
	void execute_kernel_signal(t_pcb* pcb);
	void execute_kernel_io(t_pcb* pcb);
	void execute_kernel_f_open(t_pcb* pcb);
	void execute_kernel_f_write(t_pcb* pcb);
	void execute_kernel_f_seek(t_pcb* pcb);
	void execute_kernel_f_read(t_pcb* pcb);
	void execute_kernel_f_close(t_pcb* pcb);
	void execute_kernel_f_truncate(t_pcb* pcb);
	void send_current_instruction_to_fs(t_pcb* pcb);
	void* sleepThread(void* args);

#endif
