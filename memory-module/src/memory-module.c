#include "memory-module.h"

int main(void) {
	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);
	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");


	int clientSocketId = await_client(get_logger(), serverSocketId);

	int handshakeModule = handle_handshake(clientSocketId);
	if (handshakeModule != 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Error al hacer handshake");
		return EXIT_FAILURE;
	}

	/* Handle connection */
	if(handle_connection(clientSocketId, handshakeModule)!= 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Error al manejar una conexion");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// TODO: Check if this method is necessary
char* determinate_module(int module){
	switch(module){
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
};

// TODO: Move to other file
void accept_module(void *clientSocketId, int module){
	t_operation_result result = (t_operation_result) OK;
	send(clientSocketId, &result, sizeof(t_operation_result), NULL);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("%s modulo aceptado", determinate_module(module)));
}

// TODO: Move to other file
int handle_handshake(void *clientSocketId){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Iniciando Handshake...");

	int handshake = receive_handshake(clientSocketId);
	switch (handshake) {
	case KERNEL:
		accept_module(clientSocketId, handshake);
		break;
	case CPU:
		accept_module(clientSocketId, handshake);
		break;
	case FILESYSTEM:
		accept_module(clientSocketId, handshake);
		break;
	default:
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Un modulo desconocido intento conectarse");
		send(clientSocketId, ERROR, sizeof(t_operation_result), NULL);
		return NULL;
	}
	return handshake;
}

int handle_connection(int clientSocketId, int module){

	switch(module){
	case KERNEL:;
		pthread_t kernel_thread_id;
		int err = pthread_create(&kernel_thread_id, NULL, listen_kernel_connection, &clientSocketId);
		if (err != 0){
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Error creating thread to listen kernel");
			return err;
		}
		pthread_join(kernel_thread_id,NULL);
		break;
	case CPU:
		break;
	case FILESYSTEM:
		break;
	}
	return 0;
}

// TODO: Remove this method
int receive_operation_code_2(int clientSocket) {
	int* operationCode;
	recv(clientSocket, operationCode, sizeof(int), MSG_WAITALL);
	return &operationCode;
}

// TODO: Move to other file
void* listen_kernel_connection(void* clientSocket){
	int clientSocketId = *(int*)clientSocket;
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "aaaaa");
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, string_from_format("%d", clientSocketId));
		int operationCode = receive_operation_code(clientSocketId);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Received operation code: %i", operationCode));

		switch(operationCode){
		case MESSAGE:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Recibiendo mensaje");
			break;
		case PACKAGE:
			t_list *commands = decode_package(clientSocketId);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Me llego el siguiente package:\n");
			list_iterate(commands, (void*) write_info_to_all_logs);
			break;
		case -1:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "El kernel se desconecto. Cerrando servidor");
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Operacion desconocida.");
			break;
		}
		return 0;
}
