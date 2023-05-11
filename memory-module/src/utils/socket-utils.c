#include "socket-utils.h"

t_modules_thread_id *modulesThreadId;

void handle_handshake(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Iniciando Handshake...");
	int handshake = receive_handshake(clientSocketId);
	switch (handshake) {
	case KERNEL:
		accept_module(clientSocketId, handshake);
		handle_kernel_connection(clientSocketId);
		break;
	case CPU:
		accept_module(clientSocketId, handshake);
		break;
	case FILESYSTEM:
		accept_module(clientSocketId, handshake);
		break;
	default:;
		operation_result response= OPERATION_RESULT_ERROR;
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Un modulo desconocido intento conectarse");
		send(clientSocketId, &response, sizeof(operation_result), NULL);
		exit(EXIT_FAILURE);
	}
}

char *module_as_string(module_handshakes module) {
	return moduleNames[module];
}
void init_modules(){
	modulesThreadId=malloc(sizeof(t_modules_thread_id));
}
t_modules_thread_id *get_modules_thread_id(){
	return modulesThreadId;
}

void accept_module(int clientSocketId, int module) {
	operation_result result = (operation_result) OPERATION_RESULT_OK;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("Modulo %s aceptado",module_as_string(module)));
}

void handle_kernel_connection(int clientSocketId) {
	int err = pthread_create(&modulesThreadId->kernelThread, NULL, listen_kernel_connection,clientSocketId);
	if (err != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error creating thread to listen kernel");
		exit(EXIT_FAILURE);
	}
}

void listen_kernel_connection(int clientSocketId) {
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
			exit(EXIT_FAILURE);
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Operacion desconocida.");
			break;
		}
	}
}
