#include "kernel-module.h"

typedef enum {
	MESSAGE, PACKAGE
} op_code;

int main(void) {
	t_kernel_config *KERNEL_ENV = create_kernel_config(MODULE_NAME);
	init_logger(MODULE_NAME, KERNEL_ENV->LOG_LEVEL);

	log_config(KERNEL_ENV);

	int serverSocketId = start_server(KERNEL_ENV->IP, KERNEL_ENV->PORT, getLogger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Este es un mensaje extra, no deberia estar en el archivo normal");

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
