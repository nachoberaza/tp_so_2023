#ifndef SOCKET_UTILS_H_
#define SOCKET_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <readline/readline.h>
	#include <sys/socket.h>
	#include "shared-sockets.h"
	#include "shared-logs.h"
	#include "config-utils.h"

	typedef struct {
		int size;
		void *stream;
	} t_buffer;

	typedef struct {
		op_code operationCode;
		t_buffer *buffer;
	} t_package;

	typedef struct {
		int cpu;
		int fileSystem;
		int memory;
	} t_kernel_connections;

	t_kernel_connections* start_connections(t_kernel_config* env);
	t_package* build_package(t_list* values);
	t_package* create_package(void);
	void create_buffer(t_package *package);
	void fill_package(t_package *package, void *value, int lineSize);
	void send_package(t_package *package, int clientSocket);
	void* serialize_package(t_package *package, int bytes);
	void delete_package(t_package *package);
	void fill_package_with_list(t_list* self, t_package* pkg);

#endif
