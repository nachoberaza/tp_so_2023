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

t_file_system_config* get_memory_connection() {
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

void listen_kernel_connection(int clientSocketId) {
	while(1){
		sleep(30);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection] Escuchando al modulo kernel");
	}
}

