#include "socket-utils.h"

int handle_handshake(void *clientSocket) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Iniciando Handshake...");

	int handshake = receive_handshake(clientSocket);
	switch (handshake) {
	case KERNEL:
		accept_module(clientSocket, handshake);

		int err = handle_kernel_connection(clientSocket);
		if (err != 0) {
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error handle kernel connection");
			return -1;
		}

		break;
	case CPU:
		accept_module(clientSocket, handshake);
		break;
	case FILESYSTEM:
		accept_module(clientSocket, handshake);
		break;
	default:
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Un modulo desconocido intento conectarse");
		send(clientSocket, ERROR, sizeof(t_operation_result), NULL);
		return NULL;
	}
	return handshake;
}

char* determinate_module(int module) {
	switch (module) {
	case KERNEL:
		return "KERNEL";
		break;
	case CPU:
		return "CPU";
		break;
	case FILESYSTEM:
		return "FILESYSTEM";
		break;
	}
	return "";
}
;

void accept_module(void *clientSocketId, int module) {
	t_operation_result result = (t_operation_result) OK;
	send(clientSocketId, &result, sizeof(t_operation_result), NULL);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("%s modulo aceptado",determinate_module(module)));
}

int handle_kernel_connection(void *clientSocket) {
	pthread_t kernel_thread_id;

	int err = pthread_create(&kernel_thread_id, NULL, listen_kernel_connection,&clientSocket);
	if (err != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error creating thread to listen kernel");
		return err;
	}
	pthread_join(kernel_thread_id, NULL); // TODO: Revisar si esto es necesario

	return 0;
}

void* listen_kernel_connection(void *clientSocket) {
	int clientSocketId = *(int*) clientSocket;
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,string_from_format("El valor del client socket id recibido: %i", clientSocketId));
	while(1){
		int operationCode = receive_operation_code(clientSocketId);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("Listen kernel connection -> Received operation code: %i",operationCode));

		switch (operationCode) {
		case MESSAGE:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Recibiendo mensaje");
			break;
		case PACKAGE:;
			t_list *commands = decode_package(clientSocketId);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,"Me llego el siguiente package:\n");
			list_iterate(commands, (void*) write_info_to_all_logs);
			break;
		case -1:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"El kernel se desconecto. Cerrando servidor");
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Operacion desconocida.");
			break;
		}
	}
	return 0;
}
