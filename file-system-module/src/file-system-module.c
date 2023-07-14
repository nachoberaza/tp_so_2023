#include "file-system-module.h"


int main(int argc, char** argv) {

    if (argc < 2) {
        return EXIT_FAILURE;
    }

	init_file_system_config(argv[1]);

	t_file_system_config* fileSystemEnv = get_file_system_config();
	init_logger(MODULE_NAME, fileSystemEnv->LOG_LEVEL);

	log_config(fileSystemEnv);

	int serverSocketId = start_server(fileSystemEnv->IP, fileSystemEnv->PORT, get_logger(), MODULE_NAME);

	start_memory_connection(fileSystemEnv);
	create_filesystem_structures();
	await_kernel(serverSocketId);

	return EXIT_SUCCESS;
}

