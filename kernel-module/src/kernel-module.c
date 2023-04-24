#include "kernel-module.h"

int main(void) {
	t_kernel_config *KERNEL_ENV = create_kernel_config(MODULE_NAME);

	init_logger(MODULE_NAME, KERNEL_ENV->LOG_LEVEL);
	log_config(KERNEL_ENV);

	int serverSocketId = start_server(KERNEL_ENV->IP, KERNEL_ENV->PORT, get_logger());
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Servidor de kernel listo");

	t_kernel_connections* KERNEL_CONNECTIONS = start_connections(KERNEL_ENV);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Kernel conectado a CPU, FileSystem y Memoria");

	handle_console(KERNEL_CONNECTIONS, serverSocketId);

	return EXIT_SUCCESS;
}

void handle_console(t_kernel_connections* KERNEL_CONNECTIONS, int serverSocketId) {
	//Este método no es representativo de como tiene q funcionar la app realmente, está acá para limpiar el main nomás
	int clientSocketId = await_client(get_logger(), serverSocketId);

	while (1) {
		int operationCode = receive_operation_code(clientSocketId);
		switch (operationCode) {
		case MESSAGE:
			decode_message(get_logger(),clientSocketId);
			break;
		case PACKAGE:
			t_list * lines = decode_package(clientSocketId);

			build_pcb(lines);

			send_package_to_all_targets(lines, KERNEL_CONNECTIONS);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Mande valores a FileSystem, CPU y MEMORIA!\n");

			break;
		case -1:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "La consola se desconecto. Cerrando servidor");
			exit(EXIT_FAILURE);
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Operacion desconocida.");
			break;
		}
	}
}

void send_package_to_all_targets(t_list *commands, t_kernel_connections* KERNEL_CONNECTIONS) {
	//Este método no es representativo de como tiene q funcionar la app realmente, está acá para limpiar el main nomás

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Me llego el siguiente package:\n");
	list_iterate(commands, (void*) write_info_to_all_logs);

	t_package* packageReceived = build_package(commands);
	send_package(packageReceived, KERNEL_CONNECTIONS->cpu);
	send_package(packageReceived, KERNEL_CONNECTIONS->memory);
	send_package(packageReceived, KERNEL_CONNECTIONS->fileSystem);
	delete_package(packageReceived);
}

void build_pcb(t_list *lines) {
	//Este método no es representativo de como tiene q funcionar la app realmente, está acá para limpiar el main nomás

	t_pcb* pcb = create_pcb_from_lines(lines);

	t_open_file_row* open_file_row = malloc(sizeof(t_open_file_row));
	open_file_row->file = "asd";
	open_file_row->pointer = "asd2";
	add_file(pcb, open_file_row);

	t_segment_row* segment_row = malloc(sizeof(t_segment_row));
	add_segment(pcb, segment_row);

	log_pcb(pcb);

	free_pcb(pcb);
}
