#ifndef UTILS_COMMANDS_UTILS_H_
#define UTILS_COMMANDS_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>// For sleep
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include "config-utils.h"
	#include "logger-utils.h"

	int execute_set(t_execution_context* context);
	int execute_yield(t_execution_context* context);
	int execute_exit(t_execution_context* context);
	int execute_wait(t_execution_context* context);
	int execute_signal(t_execution_context* context);
	int execute_io(t_execution_context* context);
	int execute_mov_in(t_execution_context* context);
	int execute_mov_out(t_execution_context* context);
	int execute_f_open(t_execution_context* context);
	int execute_f_close(t_execution_context* context);
	int execute_f_seek(t_execution_context* context);
	int execute_f_read(t_execution_context* context);
	int execute_f_write(t_execution_context* context);
	int execute_f_truncate(t_execution_context* context);
	int execute_create_segment(t_execution_context* context);
	int execute_delete_segment(t_execution_context* context);
	void pass_instructions_parameters_to_reason_parameters(t_instruction* currentInstruction, t_list* reasonParameters);

#endif
