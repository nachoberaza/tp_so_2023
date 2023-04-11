#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <sys/socket.h>

typedef enum {
	MESSAGE, PACKAGE
} op_code;

typedef struct {
	int size;
	void *stream;
} t_buffer;

typedef struct {
	op_code operationCode;
	t_buffer *buffer;
} t_package;

void build_package(int connection);
t_package* create_package(void);
void create_buffer(t_package *package);
void fill_package(t_package *package, void *value, int lineSize);
void send_package(t_package *package, int clientSocket);
void* serialize_package(t_package *package, int bytes);
void delete_package(t_package *package);

#endif
