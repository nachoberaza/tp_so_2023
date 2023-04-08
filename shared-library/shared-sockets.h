#ifndef SHARED_SOCKETS_H_
#define SHARED_SOCKETS_H_

	#include <stdlib.h>
	#include <commons/collections/list.h>
	#include <commons/string.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <commons/log.h>
	#include "shared-logs.h"

	int connect_to_server(char *ip, char *port);
	void close_connection(int clientSocket);
	int start_server(char* IP, char* PORT, t_log_grouping* log);
	void cleanup(int connection, t_log_grouping *logger);
	int await_client(t_log_grouping* logger, int serverSocket);
	int receive_operation_code(int clientSocket);
	void* receive_buffer(int *bufferSize, int clientSocket);
	void decode_message(t_log_grouping* logger, int clientSocket);
	t_list* decode_package(int clientSocket);

#endif
