#include "file-system-module.h"


int main(void) {
	init_file_system_config(MODULE_NAME);
	t_file_system_config* fileSystemEnv = get_file_system_config();
	init_logger(MODULE_NAME, fileSystemEnv->LOG_LEVEL);

	log_config(fileSystemEnv);

	int serverSocketId = start_server(fileSystemEnv->IP, fileSystemEnv->PORT, get_logger(), MODULE_NAME);

	return EXIT_SUCCESS;
}

