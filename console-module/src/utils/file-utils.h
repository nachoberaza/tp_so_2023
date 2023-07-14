#ifndef UTILS_FILE_UTILS_H_
#define UTILS_FILE_UTILS_H_
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/structs/socket.h>
	#include <shared/logs/log.h>
	#include <readline/readline.h>
	#include "logger-utils.h"

	void fill_buffer_from_file(t_buffer *buffer, char* instructionsPath);

#endif
