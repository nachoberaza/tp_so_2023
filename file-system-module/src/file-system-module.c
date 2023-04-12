#include "file-system-module.h"


int main(void) {
	t_file_system_config *FILE_SYSTEM_ENV = create_file_system_config(MODULE_NAME);
	init_logger(MODULE_NAME, FILE_SYSTEM_ENV->LOG_LEVEL);

	log_config(FILE_SYSTEM_ENV);

	int serverSocketId = start_server(FILE_SYSTEM_ENV->IP, FILE_SYSTEM_ENV->PORT, getLogger());

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Este es un mensaje extra, no deberia estar en el archivo normal");

	int connection = connect_to_server(FILE_SYSTEM_ENV->IP_MEMORY, FILE_SYSTEM_ENV->PORT_MEMORY);

	int clientSocketId = await_client(getLogger(), serverSocketId);

	t_list *commands;
	while (1) {
		int operationCode = receive_operation_code(clientSocketId);
		switch (operationCode) {
		case MESSAGE:
			decode_message(getLogger(),clientSocketId);
			break;
		case PACKAGE:
			commands = decode_package(clientSocketId);
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Me llegaron los siguientes valores:\n");
			list_iterate(commands, (void*) write_info_to_all_logs);
			build_package(commands, connection);
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Mande valores a un cliente!:\n");
			break;
		case -1:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_ERROR, "el cliente se desconecto. Terminando servidor");
			cleanup(connection, getLogger());
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_WARNING, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	return EXIT_SUCCESS;
}

