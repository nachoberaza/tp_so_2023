#include "socket-utils.h"

t_kernel_connections* start_connections(t_kernel_config* env) {

	t_kernel_connections* connections = malloc(sizeof(t_kernel_connections));

	connections->cpu = connect_to_server(env->IP_CPU, env->PORT_CPU);
	connections->memory = connect_to_server(env->IP_MEMORY, env->PORT_MEMORY);
	connections->fileSystem = connect_to_server(env->IP_FILESYSTEM, env->PORT_FILESYSTEM);

	operation_result memoryHandShake = init_handshake(connections->memory, (module_handshakes) KERNEL);

	if ((operation_result)memoryHandShake == OPERATION_RESULT_ERROR){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "No se pudo establecer conexion con el modulo de memoria");
		exit(EXIT_FAILURE);
	}

	return connections;
}

int handle_console_handshake(int clientSocketId) {
	int resultOk = 0, resultError = 1;
	int handshake;
	recv(clientSocketId, &handshake, sizeof(int), MSG_WAITALL);
	if (handshake != 0){
		send(clientSocketId, &resultError, sizeof(int), 0);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Consola con clientSocketId: %d, RECHAZADA", clientSocketId));
		close(clientSocketId);
		return -1;
	}

	send(clientSocketId, &resultOk, sizeof(int), 0);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Consola con clientSocketId: %d, ACEPTADA", clientSocketId));
	return handshake;
}

void listen_consoles(int serverSocketId) {
	int clientSocketId, handshakeStatus;
	t_list * lines;
	start_pcb_list();
	int cont = 0;
	while (cont < 3) {
		clientSocketId = await_client(get_logger(), serverSocketId);
		handshakeStatus = handle_console_handshake(clientSocketId);
		if(handshakeStatus != 0){
			continue;
		}
		lines = decode_package(clientSocketId);

		build_pcb(lines, clientSocketId);
		cont++;
	}
}
