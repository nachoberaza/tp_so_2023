#include "memory-module.h"

int main(void) {
	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);
	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT,get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO,"Servidor listo para recibir al cliente");

	int clientSocketId = await_client(get_logger(), serverSocketId);

	int handshakeModule = handle_handshake(clientSocketId);
	if (handshakeModule != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error al hacer handshake");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
