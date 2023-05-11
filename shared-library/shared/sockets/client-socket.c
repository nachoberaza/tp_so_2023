#include "client-socket.h"

int connect_to_server(char *ip, char *port) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, port, &hints, &server_info);

	int client_socket = socket(server_info->ai_family,
			server_info->ai_socktype, server_info->ai_protocol);

	connect(client_socket, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return client_socket;
}

t_package* create_package(void) {
	t_package *package = malloc(sizeof(t_package));
	package->operationCode = PACKAGE;
	create_package_buffer(package);
	return package;
}

void create_package_buffer(t_package *package) {
	package->buffer = malloc(sizeof(t_buffer));
	package->buffer->size = 0;
	package->buffer->stream = NULL;
}

t_package* build_package(t_list* values) {
	t_package* package = create_package();
	fill_package_with_list(values, package);
	return package;
}

void fill_package_with_list(t_list* self, t_package* pkg){
	while (self->head != NULL){
		fill_package_buffer(pkg,self->head->data, strlen(self->head->data) + 1);
		self->head = self->head->next;
	}
}

void fill_package_buffer(t_package *package, void *lineValue, int lineSize) {
	package->buffer->stream = realloc(package->buffer->stream,
			package->buffer->size + lineSize + sizeof(int));

	memcpy(package->buffer->stream + package->buffer->size, &lineSize,
			sizeof(int));
	memcpy(package->buffer->stream + package->buffer->size + sizeof(int),
			lineValue, lineSize);

	package->buffer->size += lineSize + sizeof(int);
}

void send_package(t_package *package, int clientSocket) {
	int bytes = package->buffer->size + 2 * sizeof(int);
	void *serializedPackage = serialize_package(package, bytes);

	send(clientSocket, serializedPackage, bytes, 0);

	free(serializedPackage);
}

void* serialize_package(t_package *package, int bytes) {
	void *magic = malloc(bytes);
	int offset = 0;

	memcpy(magic + offset, &(package->operationCode), sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset, &(package->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset, package->buffer->stream, package->buffer->size);
	offset += package->buffer->size;

	return magic;
}

void delete_package(t_package *package) {
	free(package->buffer->stream);
	free(package->buffer);
	free(package);
}

t_buffer* create_buffer() {
	t_buffer *buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->stream = NULL;

	return buffer;
}

operation_result init_handshake(int socket, module_handshakes module){
	operation_result result;
	module_handshakes moduleToSend= module;
	send(socket, &moduleToSend, sizeof(module_handshakes), NULL);
	recv(socket, &result, sizeof(module_handshakes), MSG_WAITALL);

	return result;
}
void fill_buffer(t_buffer *buffer, void *lineValue, int lineSize) {
	buffer->stream = realloc(buffer->stream,
			buffer->size + lineSize + sizeof(int));

	memcpy(buffer->stream + buffer->size, &lineSize,
			sizeof(int));
	memcpy(buffer->stream + buffer->size + sizeof(int),
			lineValue, lineSize);

	buffer->size += lineSize + sizeof(int);
}

void send_buffer(t_buffer *buffer, int clientSocket) {
	int bytes = buffer->size + 2 * sizeof(int);
	void *serializedBuffer = serialize_buffer(buffer, bytes);

	send(clientSocket, serializedBuffer, bytes, 0);

	free(serializedBuffer);
}

void* serialize_buffer(t_buffer *buffer, int bytes) {
	void *magic = malloc(bytes);
	int offset = 0;

	memcpy(magic + offset, &(buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(magic + offset, buffer->stream, buffer->size);
	offset += buffer->size;

	return magic;
}

