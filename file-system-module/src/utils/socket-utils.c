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