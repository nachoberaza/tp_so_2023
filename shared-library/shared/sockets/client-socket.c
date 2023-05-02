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

t_package* build_package(t_list* values) {
	t_package* package = create_package();
	fill_package_with_list(values, package);
	return package;
}

t_package* create_package(void) {
	t_package *package = malloc(sizeof(t_package));
	package->operationCode = PACKAGE;
	create_buffer(package);
	return package;
}

void create_buffer(t_package *package) {
	package->buffer = malloc(sizeof(t_buffer));
	package->buffer->size = 0;
	package->buffer->stream = NULL;
}

void fill_buffer(t_package *package, void *lineValue, int lineSize) {
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

void fill_package_with_list(t_list* self, t_package* pkg){
	while (self->head != NULL){
		fill_buffer(pkg,self->head->data, strlen(self->head->data) + 1);
		self->head = self->head->next;
	}
}

t_operation_result *init_handshake(int socket, t_module_handshakes module){
	t_module_handshakes handshake = module; //TODO: correct this
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Sending: %i", handshake));

	t_operation_result result;

	send(socket, &handshake, sizeof(t_module_handshakes), NULL);
	recv(socket, &result, sizeof(t_operation_result), MSG_WAITALL);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Recibi en el handshake: %i", result));
	return &result;
}
