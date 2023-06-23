#include "socket-utils.h"

t_modules_thread_id *modulesThreadId;

t_modules_thread_id *get_modules_thread_id() {
	return modulesThreadId;
}

void await_modules(int serverSocketId) {
	modulesThreadId = malloc(sizeof(t_modules_thread_id));

	int clientSocketId;
	for	(int i = 0; i < MODULE_ENUM_SIZE; i++){
		clientSocketId = await_client(get_logger(), serverSocketId);
		handle_handshake(clientSocketId);
	}
}

void handle_handshake(int clientSocketId) {
	int handshake = receive_handshake(clientSocketId);
	switch (handshake) {
		case KERNEL:
			accept_module(clientSocketId, handshake);
			handle_kernel_connection(clientSocketId);
			break;
		case CPU:
			accept_module(clientSocketId, handshake);
			handle_cpu_connection(clientSocketId);
			break;
		case FILESYSTEM:
			accept_module(clientSocketId, handshake);
			handle_filesystem_connection(clientSocketId);
			break;
		default:;
			operation_result response = OPERATION_RESULT_ERROR;
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"[utils/socket-utils - handle_handshake] Un modulo desconocido intento conectarse");
			send(clientSocketId, &response, sizeof(operation_result), NULL);
			exit(EXIT_FAILURE);
	}
}

void accept_module(int clientSocketId, int module) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - accept_module] Conexion recibida de %s", moduleNames[module]));
	operation_result result = OPERATION_RESULT_OK;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - accept_module] Modulo %s aceptado", moduleNames[module]));
}

void handle_kernel_connection(int clientSocketId) {
	int kernelThreadStatus = pthread_create(&modulesThreadId->kernelThread, NULL, listen_kernel_connection, clientSocketId);
	if (kernelThreadStatus != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"[utils/socket-utils - accept_module] Error creando thread para escuchar a kernel");
		exit(EXIT_FAILURE);
	}
}

void handle_cpu_connection(int clientSocketId) {
	int cpuThreadStatus = pthread_create(&modulesThreadId->cpuThread, NULL, listen_cpu_connection, clientSocketId);
	if (cpuThreadStatus != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"[utils/socket-utils - accept_module] Error creando thread para escuchar a cpu");
		exit(EXIT_FAILURE);
	}
}
void handle_filesystem_connection(int clientSocketId) {
	int filesystemThreadStatus = pthread_create(&modulesThreadId->fileSystemThread, NULL, listen_filesystem_connection, clientSocketId);
	if (filesystemThreadStatus != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"[utils/socket-utils - accept_module] Error creando thread para escuchar a filesystem");
		exit(EXIT_FAILURE);
	}
}

void listen_filesystem_connection(int clientSocketId) {
	while(1){
		sleep(30);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[utils/socket-utils - listen_filesystem_connection] Escuchando al modulo file system");
	}
}
