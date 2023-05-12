#include "memory-module.h"

int main(void) {

	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);
	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);
	init_modules();

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	for	(int i=0;i<3;i++){
		int clientSocketId = await_client(get_logger(), serverSocketId);
		handle_handshake(clientSocketId);
	}

	t_modules_thread_id *modulesThreadId = get_modules_thread_id();

	pthread_join(modulesThreadId->kernelThread,NULL); // TODO: move to a method
	pthread_join(modulesThreadId->cpuThread,NULL);
	pthread_join(modulesThreadId->fileSystemThread,NULL);

	return EXIT_SUCCESS;
}
