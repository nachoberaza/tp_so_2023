#ifndef SHARED_SOCKETS_SERVER_SOCKET_H_
#define SHARED_SOCKETS_SERVER_SOCKET_H_

	#include <stdlib.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <commons/collections/list.h>
	#include <commons/string.h>
	#include <commons/log.h>
	#include "../logs/log.h"
	#include "../structs/socket.h"


	void close_connection(int clientSocketId);
	int start_server(char* IP, char* PORT, t_log_grouping* log, char *moduleName);
	void cleanup(int connection, t_log_grouping *logger);
	int await_client(t_log_grouping* logger, int serverSocketId);
	int receive_operation_code(int clientSocketId);
	void* receive_buffer(int *bufferSize, int clientSocketId);
	int receive_handshake(int clientSocketId);
	void decode_message(t_log_grouping* logger, int clientSocketId);
	t_list* decode_package(int clientSocketId);

#endif
