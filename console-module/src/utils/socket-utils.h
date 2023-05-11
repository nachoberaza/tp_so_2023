#ifndef UTILS_SOCKET_UTILS_H_
#define UTILS_SOCKET_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/structs/socket.h>
	#include <shared/logs/log.h>
	#include <readline/readline.h>
	#include "config-utils.h"
	#include "file-utils.h"

	void connect_to_kernel(t_console_config *consoleEnv);
	int get_kernel_connection();
	void handle_kernel_handshake();
	void handle_kernel_response();
	void send_instructions_to_kernel();
#endif
