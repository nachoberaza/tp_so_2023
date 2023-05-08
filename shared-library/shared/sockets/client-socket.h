#ifndef SHARED_SOCKETS_CLIENT_SOCKET_H_
#define SHARED_SOCKETS_CLIENT_SOCKET_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <readline/readline.h>
	#include <netdb.h>
	#include <sys/socket.h>
	#include <commons/collections/list.h>
	#include "../logs/log.h"
	#include "../structs/socket.h"

	int connect_to_server(char *ip, char *port);
	t_package* build_package(t_list* values);
	t_package* create_package(void);
	void create_buffer(t_package *package);
	void fill_buffer(t_package *package, void *value, int lineSize);
	void send_package(t_package *package, int clientSocket);
	void* serialize_package(t_package *package, int bytes);
	void delete_package(t_package *package);
	void fill_package_with_list(t_list* self, t_package* pkg);
	operation_result init_handshake(int socket, module_handshakes module);

#endif
