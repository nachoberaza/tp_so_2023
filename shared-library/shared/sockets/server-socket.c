#include "server-socket.h"

void decode_message(t_log_grouping* logger, int clientSocket) {
	int bufferSize;
	char *buffer = receive_buffer(&bufferSize, clientSocket);
	write_log_grouping(logger, LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("Me llego el mensaje %s", buffer));
	free(buffer);
}

void close_connection(int clientSocket) {
	close(clientSocket);
}

void cleanup(int connection, t_log_grouping *logger) {
	log_destroy(logger->main);
	log_destroy(logger->internal);
	close_connection(connection);
}

int start_server(char* IP, char* PORT, t_log_grouping* loggers) {
	struct addrinfo hints, *serverInfo;
	int serverSocket;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PORT, &hints, &serverInfo);

	serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol);

	bind(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);

	listen(serverSocket, SOMAXCONN);

	freeaddrinfo(serverInfo);
	write_log_grouping(loggers, LOG_TARGET_ALL, LOG_LEVEL_INFO, "Listo para escuchar a mi cliente");

	return serverSocket;
}

int await_client(t_log_grouping* logger, int serverSocket) {
	// Aceptamos un nuevo cliente
	int clientSocket = accept(serverSocket, NULL, NULL);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("serverSocketId: [%d]", serverSocket));
	write_log_grouping(logger, LOG_TARGET_ALL, LOG_LEVEL_INFO, "Se conecto un cliente!");

	return clientSocket;
}

int receive_operation_code(int clientSocket) {
	int operationCode;
	if (recv(clientSocket, &operationCode, sizeof(int), MSG_WAITALL) > 0)
		return operationCode;
	else {
		close(clientSocket);
		return -1;
	}
}

void* receive_buffer(int *bufferSize, int clientSocket) {
	void *buffer;

	recv(clientSocket, bufferSize, sizeof(int), MSG_WAITALL);
	buffer = malloc(*bufferSize);
	recv(clientSocket, buffer, *bufferSize, MSG_WAITALL);

	return buffer;
}

int receive_handshake(int clientSocket){
	int handshake;
	recv(clientSocket, &handshake, sizeof(int), MSG_WAITALL);
	return handshake;
}

t_list* decode_package(int clientSocket) {
	int bufferSize, valueSize, offset = 0;
	void *buffer;
	t_list *values = list_create();

	buffer = receive_buffer(&bufferSize, clientSocket);
	while (offset < bufferSize) {
		memcpy(&valueSize, buffer + offset, sizeof(int));
		offset += sizeof(int);
		char *value = malloc(valueSize);
		memcpy(value, buffer + offset, valueSize);
		offset += valueSize;
		list_add(values, value);
	}
	free(buffer);
	return values;
}

