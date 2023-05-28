#include "socket-utils.h"

t_kernel_connections* kernelConnections;

void start_connections(t_kernel_config* env) {

	kernelConnections = malloc(sizeof(t_kernel_connections));

	kernelConnections->cpu = connect_to_server(env->IP_CPU, env->PORT_CPU);
	kernelConnections->memory = connect_to_server(env->IP_MEMORY, env->PORT_MEMORY);
	kernelConnections->fileSystem = connect_to_server(env->IP_FILESYSTEM, env->PORT_FILESYSTEM);

	operation_result memoryHandShake = init_handshake(kernelConnections->memory,(module_handshakes) KERNEL);

	if (memoryHandShake == OPERATION_RESULT_ERROR){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/socket-utils - start_connections] No se pudo establecer conexion con el modulo de memoria");
		exit(EXIT_FAILURE);
	}
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - start_connections] CPU Socket: %d", kernelConnections->cpu));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - start_connections] Kernel conectado a CPU, FileSystem y Memoria");
}

t_kernel_connections* get_kernel_connections() {
	return kernelConnections;
}

int get_cpu_connection() {
	return kernelConnections->cpu;
}

int get_file_system_connection() {
	return kernelConnections->fileSystem;
}

int get_memory_connection() {
	return kernelConnections->memory;
}

int handle_console_handshake(int clientSocketId) {
	operation_result resultOk = OPERATION_RESULT_OK, resultError = OPERATION_RESULT_ERROR;
	operation_result handshakeResult;
	recv(clientSocketId, &handshakeResult, sizeof(int), MSG_WAITALL);
	if (handshakeResult != OPERATION_RESULT_OK){
		send(clientSocketId, &resultError, sizeof(operation_result), 0);
		write_to_log(
				LOG_TARGET_INTERNAL,
				LOG_LEVEL_DEBUG,
				string_from_format("[utils/socket-utils - handle_console_handshake] Consola con clientSocketId: %d - RECHAZADA", clientSocketId)
		);
		close(clientSocketId);
		return -1;
	}

	send(clientSocketId, &resultOk, sizeof(operation_result), 0);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/socket-utils - handle_console_handshake] Consola con clientSocketId: %d, ACEPTADA", clientSocketId));
	return handshakeResult;
}

void listen_consoles(int serverSocketId) {
	int clientSocketId, handshakeStatus;
	t_list * lines;
	start_pcb_list();
	while (1) {
		clientSocketId = await_client(get_logger(), serverSocketId);
		handshakeStatus = handle_console_handshake(clientSocketId);
		if(handshakeStatus != 0){
			continue;
		}
		lines = decode_package(clientSocketId);

		build_pcb(lines, clientSocketId);

		execute_long_term_scheduler();
	}
}

void execute_long_term_scheduler(){
	//TODO: Reescribir
	int size = list_size(get_new_pcb_list());
	for (int i=0; i<size; i++){
		t_pcb* pcb = list_get(get_new_pcb_list(), 0);
		pcb->state = READY;
		list_add(get_short_term_list(), pcb);
	}

	list_clean(get_new_pcb_list());
}
