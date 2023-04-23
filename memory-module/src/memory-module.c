#include "memory-module.h"

pthread_t tid[2];

int main(void) {
	t_memory_config *MEMORY_ENV = create_memory_config(MODULE_NAME);
	init_logger(MODULE_NAME, MEMORY_ENV->LOG_LEVEL);

	log_config(MEMORY_ENV);

	int serverSocketId = start_server(MEMORY_ENV->IP, MEMORY_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	for	(int i=0;i<3;i++)
	{
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Inside %d", i));
		int clientSocketId = await_client(get_logger(), serverSocketId);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Me llego algo: %d", clientSocketId));

		int handshake = handle_handshake(clientSocketId);
		if (handshake != 0){
			continue;
		}

		// Handle connection
	    int err = pthread_create(&(tid[i]), NULL, handleConnection, (void*) &clientSocketId);
	    if (err != 0){
	    	printf("\nHubo un problema al crear el thread :[%s]", strerror(err));
	    	return err;
	    }
	}

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

	return EXIT_SUCCESS;
}

int handle_handshake(int socket){
	uint32_t resultOk = 0;
	uint32_t resultError = -1;

	int handshake = receive_handshake(socket);
	switch (handshake) {
	case KERNEL:
		send(socket, &resultOk, sizeof(uint32_t), NULL);
		break;
	case CPU:
		send(socket, &resultOk, sizeof(uint32_t), NULL);
		break;
	case FILESYSTEM:
		send(socket, &resultOk, sizeof(uint32_t), NULL);
		break;
	default:
		printf("%i\n", handshake);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Un modulo desconocido intento conectarse");
		send(socket, &resultError, sizeof(uint32_t), NULL);
		return -1;
	}

	return 0;
}

void* handleConnection(void *clientSocket)
{
    int clientSocketId = *(int*)clientSocket;
	t_list *commands;
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("clientsocket inside handle: %d", clientSocketId));

	int flag = 1;

	while (flag) {
		int operationCode = receive_operation_code(clientSocketId);
		switch (operationCode) {
		case MESSAGE:
			decode_message(get_logger(),clientSocketId);
			break;
		case PACKAGE:
			commands = decode_package(clientSocketId);
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("clientSocket %d, Me llegaron los siguientes valores:\n", clientSocketId));
			list_iterate(commands, (void*) write_info_to_all_logs);
			break;
		case -1:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_ERROR, "el cliente se desconecto. Terminando servidor");
			flag = 0;
			break;
		default:
			write_to_log(LOG_TARGET_ALL, LOG_LEVEL_WARNING, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

}
