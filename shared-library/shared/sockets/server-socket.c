#include "server-socket.h"

void decode_message(t_log_grouping* logger, int clientSocketId) {
	int bufferSize;
	char *buffer = receive_buffer(&bufferSize, clientSocketId);
	write_log_grouping(logger, LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[shared/sockets/server-socket - decode_message] Me llego el mensaje %s", buffer));
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

int start_server(char* IP, char* PORT, t_log_grouping* loggers, char *moduleName) {
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
	write_log_grouping(
		loggers,
		LOG_TARGET_ALL,
		LOG_LEVEL_INFO,
		string_from_format("[shared/sockets/server-socket - start_server] %s escuchando clientes", moduleName)
	);

	return serverSocket;
}

int await_client(t_log_grouping* logger, int serverSocketId) {
	int clientSocketId = accept(serverSocketId, NULL, NULL);
	write_log_grouping(logger, LOG_TARGET_ALL, LOG_LEVEL_INFO, "[shared/sockets/server-socket - await_client] Se conecto un cliente");

	return clientSocketId;
}

int receive_operation_code(int clientSocketId) {
	int operationCode;
	if (recv(clientSocketId, &operationCode, sizeof(int), MSG_WAITALL) > 0)
		return operationCode;
	else {
		close(clientSocketId);
		return -1;
	}
}

void* receive_buffer(int *bufferSize, int clientSocketId) {
	void *buffer;

	recv(clientSocketId, bufferSize, sizeof(int), MSG_WAITALL);
	buffer = malloc(*bufferSize);
	recv(clientSocketId, buffer, *bufferSize, MSG_WAITALL);

	return buffer;
}

int receive_handshake(int clientSocketId){
	int handshake;
	recv(clientSocketId, &handshake, sizeof(int), MSG_WAITALL);
	return handshake;
}

t_list* decode_package(int clientSocketId) {
	int bufferSize, valueSize, offset = 0;
	void *buffer;
	t_list *values = list_create();

	buffer = receive_buffer(&bufferSize, clientSocketId);
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

