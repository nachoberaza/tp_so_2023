#include "memory-module.h"

int main(int argc, char** argv) {

    if (argc < 2) {
        return EXIT_FAILURE;
    }

	create_memory_config(argv[1]);

	t_memory_config *memoryEnv = get_memory_config();

	init_logger(MODULE_NAME, memoryEnv->LOG_LEVEL);

	log_config(memoryEnv);

	int serverSocketId = start_server(memoryEnv->IP, memoryEnv->PORT, get_logger(), MODULE_NAME);
	create_memory_structures();
	await_modules(serverSocketId);

	t_modules_thread_id *modulesThreadId = get_modules_thread_id();

	pthread_join(modulesThreadId->kernelThread,NULL);
	pthread_join(modulesThreadId->cpuThread,NULL);
	pthread_join(modulesThreadId->fileSystemThread,NULL);

	return EXIT_SUCCESS;
}
