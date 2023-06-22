#include "cpu-communication-utils.h"

void listen_cpu_connection(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_cpu_connection] Escuchando al modulo cpu");
	while (1) {
		//Modular a gusto
		int opCode = receive_operation_code(clientSocketId);

		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - listen_cpu_connection] Recibi operation code %d", opCode));

		int bufferSize, offset = 0;
		void *buffer;

		t_instruction* instruction;

		buffer = receive_buffer(&bufferSize, clientSocketId);

		instruction = extract_instruction_from_buffer(get_logger(), LOG_LEVEL_TRACE, buffer, &offset);

		write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, instruction);
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
	int address = atoi(list_get(instruction->parameters, 0));
	int size = atoi(list_get(instruction->parameters, 1));

	char* value = malloc(size);

	memcpy(value, get_memory() + address, size);


	t_package* package = create_package();

	fill_buffer(package->buffer, value, size);
	send_package(package, clientSocketId);
}

void execute_memory_mov_out(t_instruction* instruction, int clientSocketId){
	int address = atoi(list_get(instruction->parameters, 0));
	char* value = list_get(instruction->parameters, 1);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - listen_cpu_connection] memory: %p", get_memory()));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - listen_cpu_connection] address: %d", address));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - listen_cpu_connection] memory + address: %p", get_memory() + address));
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - listen_cpu_connection] string_length: %d", string_length(value)));

	void* destination = get_memory() + address;

	memcpy(destination, value, strlen(value) + 1);

	operation_result result;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
}


