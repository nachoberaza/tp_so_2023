#include "kernel-module.h"

int main(void) {
	t_kernel_config *kernelEnv = create_kernel_config(MODULE_NAME);

	init_logger(MODULE_NAME, kernelEnv->LOG_LEVEL);

	log_config(kernelEnv);

	init_semaphores();

	build_resources_list(kernelEnv->RESOURCES,kernelEnv->RESOURCES_INSTANCES);

	int serverSocketId = start_server(kernelEnv->IP, kernelEnv->PORT, get_logger(), MODULE_NAME);

	start_connections(kernelEnv);

	create_listener_thread(serverSocketId);

	handle_processes();

	pthread_join(get_listener_thread(), NULL);
	return EXIT_SUCCESS;
}
