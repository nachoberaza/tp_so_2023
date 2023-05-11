#include "socket-utils.h"


void add_pthread_id(pthread_t *module_thread, pthread_t *modules_thread_id){
	pthread_t module_thread_id=*(pthread_t *)module_thread;
	for(int i = 0; i < MODULE_ENUM_SIZE; i++){ //TODO: Mejorar método
		if(modules_thread_id[i]==NULL){
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Agregue un thread id a la lista");
			modules_thread_id[i]=module_thread_id;
			i=MODULE_ENUM_SIZE;
		}
	}
}


void handle_handshake(int clientSocketId, pthread_t *modules_thread_id) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Iniciando Handshake...");
	int handshake = receive_handshake(clientSocketId);
	switch (handshake) {
	case KERNEL:
		accept_module(clientSocketId, handshake);
		pthread_t kernel_thread_id = handle_kernel_connection(&clientSocketId);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("El valor del thread id a agregar es %li",kernel_thread_id));
		add_pthread_id(&kernel_thread_id, modules_thread_id);
		break;
	case CPU:
		accept_module(clientSocketId, handshake);
		break;
	case FILESYSTEM:
		accept_module(clientSocketId, handshake);
		break;
	default:
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Un modulo desconocido intento conectarse");
		send(clientSocketId, OPERATION_RESULT_ERROR, sizeof(operation_result), NULL);
		exit(EXIT_FAILURE);
	}
}

char * module_as_string(module_handshakes module) {
	return moduleNames[module];
}

void accept_module(int clientSocketId, int module) {
	operation_result result = (operation_result) OPERATION_RESULT_OK;
	send(clientSocketId, &result, sizeof(operation_result), NULL);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("El valor del client id aceptado es %i",clientSocketId));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("Modulo %s aceptado",module_as_string(module)));
}

pthread_t handle_kernel_connection(int *clientSocketId) {
	pthread_t kernel_thread_id;

	int err = pthread_create(&kernel_thread_id, NULL, listen_kernel_connection,(void*) clientSocketId);
	if (err != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error creating thread to listen kernel");
		exit(EXIT_FAILURE);
	}

	return kernel_thread_id;
}

void* listen_kernel_connection(void *clientSocket) {
	int clientSocketId = *(int*) clientSocket;
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
