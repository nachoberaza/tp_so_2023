#ifndef SOCKET_SHARED_CLIENT_SOCKETS_H_
#define SOCKET_SHARED_CLIENT_SOCKETS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/collections/list.h>
	#include <string.h>
	#include <readline/readline.h>
	#include <sys/socket.h>
	#include "../log/shared-logs.h"
	#include "../structs/socket.h"

	t_package* build_package(t_list* values);
	t_package* create_package(void);
	void create_buffer(t_package *package);
	void fill_package(t_package *package, void *value, int lineSize);
	void send_package(t_package *package, int clientSocket);
	void* serialize_package(t_package *package, int bytes);
	void delete_package(t_package *package);
	void fill_package_with_list(t_list* self, t_package* pkg);

#endif
