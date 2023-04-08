#include "socket-utils.h"

void build_package(int clientSocketId) {
	//Not optimal but this part needs to be changed to read from a file and validate format so no point in wasting time
	//This is just to be able to test something now
	char *line;
	t_package *package;
	int lineSize;

	package = create_package();

	line = readline("> ");
	do {
		lineSize = strlen(line) + 1;
		fill_package(package, line, lineSize);

		line = readline("> ");
	} while (strcmp(line, "\0"));

	send_package(package, clientSocketId);

	delete_package(package);
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

void fill_package(t_package *package, void *lineValue, int lineSize) {
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
