#include "kernel-module.h"

int main(int argc, char** argv) {

    if (argc < 2) {
        return EXIT_FAILURE;
    }

	init_kernel_config(argv[1]);
	t_kernel_config* kernelEnv = get_kernel_config();
	init_logger(MODULE_NAME, kernelEnv->LOG_LEVEL);

	log_config(kernelEnv);

	init_semaphores();

	build_resources_list(kernelEnv->RESOURCES,kernelEnv->RESOURCES_INSTANCES);
	start_open_files_table_list();

	int serverSocketId = start_server(kernelEnv->IP, kernelEnv->PORT, get_logger(), MODULE_NAME);

	start_connections(kernelEnv);

	create_listener_thread(serverSocketId);

	handle_processes();

	pthread_join(get_listener_thread(), NULL);

	return EXIT_SUCCESS;
}
