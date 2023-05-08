#include "memory-module.h"



int main(void) {
	pthread_t *modules_thread_id;
	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);
	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	int clientSocketId = await_client(get_logger(), serverSocketId);

	modules_thread_id= handle_handshake(clientSocketId);

	pthread_join(modules_thread_id[0],NULL);
	return EXIT_SUCCESS;
}
