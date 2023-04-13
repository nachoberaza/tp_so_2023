#include "kernel-module.h"

int main(void) {
	t_kernel_config *KERNEL_ENV = create_kernel_config(MODULE_NAME);
	init_logger(MODULE_NAME, KERNEL_ENV->LOG_LEVEL);

	log_config(KERNEL_ENV);

	int serverSocketId = start_server(KERNEL_ENV->IP, KERNEL_ENV->PORT, getLogger());
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	t_kernel_connections* KERNEL_CONNECTIONS = start_connections(KERNEL_ENV);
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Kernel conectado a CPU, FileSystem y Memoria");
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
			t_package* packageReceived = build_package(commands);
			send_package(packageReceived, KERNEL_CONNECTIONS->cpu);
			send_package(packageReceived, KERNEL_CONNECTIONS->memory);
			send_package(packageReceived, KERNEL_CONNECTIONS->fileSystem);
			delete_package(packageReceived);
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Mande valores a FileSystem, CPU y MEMORIA!\n");
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
