#ifndef SHARED_EXECUTION_CONTEXT_H_
#define SHARED_EXECUTION_CONTEXT_H_

	#include <stdlib.h>
	#include "../logs/log.h"
	#include "../structs/execution-context.h"
	#include "../serialization/serialization.h"
	#include "../sockets/server-socket.h"
	#include "../sockets/client-socket.h"


	t_execution_context* init_execution_context(int pid);
	void log_context(t_log_grouping* logger, t_log_level logLevel, t_execution_context* context);
	void write_instructions_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, t_list* instructions);
	void write_instruction_to_internal_log(t_log_grouping* logger, t_log_level logLevel, t_instruction* instruction);
	void write_parameter_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, char* parameter);
	void write_execution_context_reason_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, t_execution_context_reason* reason);
	void write_segment_row_to_internal_logs(t_segment_row* segmentRow);
	void destroy_instruction(t_instruction* instruction);
	void destroy_instructions(t_log_grouping* logger,t_list* instructions) ;
	void destroy_execution_context(t_log_grouping* logger, t_execution_context* executionContext);
	void destroy_execution_context_reason(t_log_grouping* logger, t_execution_context_reason* reason);
	void destroy_segment_row(t_segment_row* segmentRow);
	command command_from_string(t_log_grouping* logger, char * command);
	char * command_as_string(command command);
	char * execution_context_state_as_string(execution_context_state executionContextState);
	int get_amount_of_bytes_per_register(char* reg, t_execution_context* context);
	t_execution_context* decode_context(t_log_grouping* logger, t_log_level logLevel, int clientSocket);
	t_list* extract_instructions_from_buffer(t_log_grouping* logger, t_log_level logLevel, void * buffer, int* offset);
	t_instruction* extract_instruction_from_buffer(t_log_grouping* logger, t_log_level logLevel, void * buffer, int* offset);
	void extract_instruction_parameters_from_buffer(t_list* parameters, void * buffer, int* offset);
	t_cpu_register* extract_cpu_register_from_buffer(void* buffer, int* offset);
	t_execution_context_reason* extract_execution_context_reason_from_buffer(void * buffer, int* offset);
	execution_context_state extract_execution_context_state_from_buffer(void * buffer, int* offset);
	t_list* extract_segment_table_from_buffer(void* buffer, int *offset);
	void fill_package_with_context(t_log_grouping* logger,t_execution_context* context, t_package* pkg);
	void fill_buffer_with_instructions(t_log_grouping* logger,t_list* instructions, t_package* pkg);
	void fill_buffer_with_cpu_register(t_cpu_register* cpuRegisters, t_package* pkg);
	void fill_buffer_with_execution_context_reason(t_log_grouping* logger,t_execution_context_reason* reason, t_package* pkg);
	void fill_package_with_segment_table(t_package* pkg, t_list* segmentTable);
	void fill_buffer_with_instruction(t_instruction* instruction, t_package* pkg);
	error error_from_string(t_log_grouping* logger, char * error);
	char * error_as_string(error reason);
	void add_error_in_execution_context_reason(t_execution_context_reason* reason , execution_context_state state , error err);
	char* get_string_array_instruction_parameters(t_instruction* instruction);
	t_instruction* duplicate_instruction(t_instruction* currentInstruction);

#endif
