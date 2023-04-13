#include "cpu-module.h"


int main(void) {
	t_cpu_config *CPU_ENV = create_cpu_config(MODULE_NAME);
	init_logger(MODULE_NAME, CPU_ENV->LOG_LEVEL);

	log_config(CPU_ENV);

	int serverSocketId = start_server(CPU_ENV->IP, CPU_ENV->PORT, getLogger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

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
			break;
		case -1:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_ERROR, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_WARNING, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	return EXIT_SUCCESS;
}
