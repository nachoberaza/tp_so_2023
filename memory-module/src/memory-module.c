#include "memory-module.h"

int main(void) {
	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);

	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);

	allocate_modules_threads();

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT, get_logger(), MODULE_NAME);

	await_modules(serverSocketId);

	t_modules_thread_id *modulesThreadId = get_modules_thread_id();

	pthread_join(modulesThreadId->kernelThread,NULL);
	pthread_join(modulesThreadId->cpuThread,NULL);
	pthread_join(modulesThreadId->fileSystemThread,NULL);

	return EXIT_SUCCESS;
}
