#include "cpu-communication-utils.h"

void listen_cpu_connection(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_cpu_connection] Escuchando al modulo cpu");
	while (1) {
		//Modular a gusto
		receive_operation_code(clientSocketId);

		int bufferSize, offset = 0;
		void *buffer;

		t_instruction* instruction = malloc(sizeof(t_instruction));

		buffer = receive_buffer(&bufferSize, clientSocketId);

		instruction = extract_instruction_from_buffer(get_logger(), LOG_LEVEL_TRACE, buffer, &offset);

		write_log_grouping(get_logger(), LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, command_as_string(instruction->command));

		for (int i=0; i < list_size(instruction->parameters); i++){
			write_parameter_to_internal_logs(get_logger(), LOG_LEVEL_INFO, list_get(instruction->parameters, i));
		}

		free(buffer);


		switch (instruction->command){
			case MOV_IN:
				execute_memory_mov_in(instruction,clientSocketId);
				break;
			case MOV_OUT:
				execute_memory_mov_out(instruction,clientSocketId);
				break;
			default:
				write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_cpu_connection] Comando no pertenece a Memory");
				return;
			break;
		}

	}
}

void execute_memory_mov_in(t_instruction* instruction, int clientSocketId){
	operation_result result;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
}

void execute_memory_mov_out(t_instruction* instruction, int clientSocketId){
	operation_result result;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
}




