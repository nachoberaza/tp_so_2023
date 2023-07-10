#include "socket-utils.h"

int memoryConnection;

void start_memory_connection(t_file_system_config* fileSystemEnv) {

	memoryConnection = connect_to_server(fileSystemEnv->IP_MEMORY, fileSystemEnv->PORT_MEMORY);

	operation_result memoryHandShake = init_handshake(memoryConnection,(module_handshakes) FILESYSTEM);

	if (memoryHandShake == OPERATION_RESULT_ERROR){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/socket-utils - start_memory_connection] No se pudo establecer conexion con el modulo de memoria");
		exit(EXIT_FAILURE);
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - start_memory_connection] FileSystem conectado a Memoria");
}

int get_memory_connection() {
	return memoryConnection;
}

void await_kernel(int serverSocketId) {
	int clientSocketId = await_client(get_logger(), serverSocketId);
	handle_handshake(clientSocketId);
}

void handle_handshake(int clientSocketId) {
	int handshake = receive_handshake(clientSocketId);
	operation_result result = OPERATION_RESULT_ERROR;
	switch (handshake) {
		case KERNEL:
			result = OPERATION_RESULT_OK;
			send(clientSocketId, &result, sizeof(operation_result), NULL);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - accept_module] Modulo Kernel aceptado"));
			break;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"[utils/socket-utils - handle_handshake] Un modulo desconocido intento conectarse");
			send(clientSocketId, &result, sizeof(operation_result), NULL);
			exit(EXIT_FAILURE);
	}

	listen_kernel_connection(clientSocketId);
}

t_instruction* decode_instruction(int clientSocketId){

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

	return instruction;
}

void listen_kernel_connection(int clientSocketId) {
	while(1){
			operation_code operationCode = receive_operation_code(clientSocketId);
			write_log_grouping(get_logger(), LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("kernel thread - Operation code : %d", operationCode));

			t_memory_data* memoryData = decode_memory_data(get_logger(), clientSocketId);
			/*
			*	La respuesta en realidad varia dedpendiendo del método,
			*	Ahora manda solo un result pero deberiamos tener las N posibles respuesta
			*	Y recibirlas/decodearlas del lado del kernel
			*/

			log_memory_data(memoryData, get_logger(), LOG_LEVEL_DEBUG);

			operation_result result;

			switch (memoryData->instruction->command){
				case F_CREATE:
					result = execute_fs_f_create(memoryData->instruction);
					break;
				case F_OPEN:
					result = execute_fs_f_open(memoryData->instruction);
					break;
				case F_READ:
					result = execute_fs_f_read(memoryData);
					break;
				case F_WRITE:
					result = execute_fs_f_write(memoryData);
					break;
				case F_TRUNCATE:
					result = execute_fs_f_truncate(memoryData->instruction);
					break;
				default:
					result = OPERATION_RESULT_ERROR;
					write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection] Comando no pertenece al FS");
					break;
			}
			send(clientSocketId, &result, sizeof(operation_result), NULL);
	}
}





