#include "kernel-communication-utils.h"

void listen_kernel_connection(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Escuchando al modulo kernel");
	while(1){
		operation_code operationCode = receive_operation_code(clientSocketId);
		write_log_grouping(get_logger(), LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("kernel thread - Operation code : %d", operationCode));
		switch(operationCode){
			case CREATE_SEGMENT_TABLE:
				execute_memory_create_segment_table(clientSocketId);
				break;
			case DELETE_SEGMENT_TABLE:
				execute_memory_delete_segment_table(clientSocketId);
				break;
			case COMPRESS_SEGMENT_TABLE:
				execute_memory_compress_segment_table(clientSocketId);
				break;
			case PACKAGE:
				execute_memory_kernel_instruction(clientSocketId);
				break;
			default:
				write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando DEFAULT");
				break;
		}
	}
}

void execute_memory_kernel_instruction(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando PACKAGE");

	t_instruction* instruction = malloc(sizeof(t_instruction));

	buffer = receive_buffer(&bufferSize, clientSocketId);

	instruction = extract_instruction_from_buffer(get_logger(), LOG_LEVEL_TRACE, buffer, &offset);

	write_log_grouping(get_logger(), LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, command_as_string(instruction->command));

	for (int i=0; i < list_size(instruction->parameters); i++){
		write_log_grouping(get_logger(), LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, list_get(instruction->parameters, i));
	}

	free(buffer);

	switch (instruction->command){
		case CREATE_SEGMENT:
			execute_memory_create_segment(instruction,clientSocketId);
			break;
		case DELETE_SEGMENT:
			execute_memory_delete_segment(instruction,clientSocketId);
			break;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection] Comando no pertenece a Memory");
			return;
		break;
	}
}

void execute_memory_compress_segment_table(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando COMPRESS_SEGMENT_TABLE");
	//TODO
}

void execute_memory_delete_segment_table(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando DELETE_SEGMENT_TABLE");
	buffer = receive_buffer(&bufferSize, clientSocketId);
	int pid = extract_int_from_buffer(buffer, &offset);

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO, string_from_format("Eliminación de Proceso PID: %d",pid));

	operation_result result;

	send(clientSocketId, &result, sizeof(operation_result), NULL);

	//TODO
}

void execute_memory_create_segment_table(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando CREATE_SEGMENT_TABLE");

	buffer = receive_buffer(&bufferSize, clientSocketId);
	int pid = extract_int_from_buffer(buffer, &offset);

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO, string_from_format("Creación de Proceso PID: %d",pid));

	t_list* segmentTable = create_segment_table();
	t_package* package = create_package();
	fill_package_with_segment_table(package, segmentTable);
	send_package(package, clientSocketId);
}

void execute_memory_create_segment(t_instruction* instruction, int clientSocketId){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_memory_create_segment] Ejecutando create segment");
	operation_result result;

	send(clientSocketId, &result, sizeof(operation_result), NULL);
}

void execute_memory_delete_segment(t_instruction* instruction, int clientSocketId){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_memory_delete_segment] Ejecutando delete segment");
	operation_result result;

	send(clientSocketId, &result, sizeof(operation_result), NULL);
}

