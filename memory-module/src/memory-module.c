#include "memory-module.h"


void acceptModule(void *socketId, t_module_handshakes *module);

void handle_connection(int clientSocketID, t_module_handshakes *module);

int main(void) {
	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);
	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	while(1){
		int clientSocketId = await_client(get_logger(), serverSocketId);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Me llego algo: %d", clientSocketId));

		t_module_handshakes* handshakeModule = handle_handshake(clientSocketId);
		if (handshakeModule == NULL){
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Error al hacer handshake");
			return EXIT_FAILURE;
		}

		// handle_connection
		handle_connection(clientSocketId, handshakeModule);
	}

	return EXIT_SUCCESS;
}

// TODO: Move to other file
void acceptModule(void *socketId, t_module_handshakes *module){
	send(socketId, OK, sizeof(int), NULL); // TODO: send pointer of t_operation_result
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, string_from_format("Accept a %p module", *module)); // TODO: print pointer struct value
}

// TODO: Move to other file
t_module_handshakes* handle_handshake(void *socketId){
	t_module_handshakes* handshake = receive_handshake(socketId);
	switch (*handshake) {
	case KERNEL:
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Handshake ok with a kernel module");
		acceptModule(socketId, handshake);
		break;
	case CPU:
		send(socket, OK, sizeof(int), NULL);
		break;
	case FILESYSTEM:
		send(socket, OK, sizeof(int), NULL);
		break;
	default:
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Un modulo desconocido intento conectarse");
		send(socket, ERROR, sizeof(int), NULL);
		return NULL;
	}

	return handshake;
}

void handle_connection(int clientSocketId, t_module_handshakes *module){

	switch(*module){
	case KERNEL:;

		pthread_t kernel_thread_id;
		int err = pthread_create(&kernel_thread_id, NULL, listen_kernel_connection, (void*) &clientSocketId);
		if (err != 0){
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Error creating thread to listen kernel");
			return err;
		}

		break;
	case CPU:
		break;
	case FILESYSTEM:
		break;
	}
}

// TODO: Move to other file
int listen_kernel_connection(void *clientSocket){
	while(1){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Listen a kernel module...");

		int operationCode = receive_operation_code(clientSocket);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Received operation code: %d", operationCode));

		switch(operationCode){
		case MESSAGE:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Recibiendo mensaje");
			break;
		case -1:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "La consola se desconecto. Cerrando servidor");
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Operacion desconocida.");
			break;
		}
	}
}
