#include "file-system-module.h"


int main(void) {
	t_file_system_config *FILE_SYSTEM_ENV = create_file_system_config(MODULE_NAME);
	init_logger(MODULE_NAME, FILE_SYSTEM_ENV->LOG_LEVEL);

	log_config(FILE_SYSTEM_ENV);

	int serverSocketId = start_server(FILE_SYSTEM_ENV->IP, FILE_SYSTEM_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	int memoryConnection = connect_to_server(FILE_SYSTEM_ENV->IP_MEMORY, FILE_SYSTEM_ENV->PORT_MEMORY);
	handle_memory_handshake(memoryConnection,FILESYSTEM);

	int clientSocketId = await_client(get_logger(), serverSocketId);

	t_list *commands;
	while (1) {
		int operationCode = receive_operation_code(clientSocketId);
		switch (operationCode) {
		case MESSAGE:
			decode_message(get_logger(),clientSocketId);
			break;
		case PACKAGE:
			commands = decode_package(clientSocketId);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Me llego el siguiente package:\n");
			list_iterate(commands, (void*) write_info_to_all_logs);
			t_package* packageReceived = build_package(commands);
			send_package(packageReceived, memoryConnection);
			delete_package(packageReceived);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Mande valores a MEMORIA!\n");
			break;
		case -1:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_ERROR, "el cliente se desconecto. Terminando servidor");
			cleanup(memoryConnection, get_logger());
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_WARNING, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	return EXIT_SUCCESS;
}

