#include "file-system-module.h"


int main(void) {
	init_file_system_config(MODULE_NAME);
	t_file_system_config* fileSystemEnv = get_file_system_config();
	init_logger(MODULE_NAME, fileSystemEnv->LOG_LEVEL);

	log_config(fileSystemEnv);

	int serverSocketId = start_server(fileSystemEnv->IP, fileSystemEnv->PORT, get_logger(), MODULE_NAME);
	
	start_memory_connection(fileSystemEnv);
	
	//int memoryConnection = connect_to_server(FILE_SYSTEM_ENV->IP_MEMORY, FILE_SYSTEM_ENV->PORT_MEMORY);


	return EXIT_SUCCESS;
}

