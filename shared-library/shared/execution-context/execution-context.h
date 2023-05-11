#ifndef SHARED_EXECUTION_CONTEXT_H_
#define SHARED_EXECUTION_CONTEXT_H_

	#include <stdlib.h>
	#include "../logs/log.h"
	#include "../structs/execution-context.h"
	#include "../serialization/serialization.h"
	#include "../sockets/server-socket.h"
	#include "../sockets/client-socket.h"


	void log_context(t_log_grouping* logger, t_log_level logLevel, t_execution_context* context);
	void write_instructions_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, t_list* instructions);
	void write_parameter_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, char* parameter);
	void destroy_instruction(t_instruction* instruction);
	void destroy_instructions(t_log_grouping* logger,t_list* instructions) ;
	void destroy_execution_context(t_log_grouping* logger, t_execution_context* executionContext);
	command command_from_string(t_log_grouping* logger, char * command);
	char * command_as_string(command command);
	t_execution_context* decode_context(t_log_grouping* logger, t_log_level logLevel, int clientSocket);
	t_list* extract_instructions_from_buffer(t_log_grouping* logger, t_log_level logLevel, void * buffer, int* offset);
	t_cpu_register* extract_cpu_register_from_buffer(void* buffer, int* offset);
	void fill_package_with_context(t_log_grouping* logger,t_execution_context* context, t_package* pkg);
	void fill_buffer_with_instructions(t_log_grouping* logger,t_list* instructions, t_package* pkg);
	void fill_buffer_with_cpu_register(t_cpu_register* cpuRegisters, t_package* pkg);

#endif
