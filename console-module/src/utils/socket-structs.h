#ifndef SOCKET_STRUCTS_H_
#define SOCKET_STRUCTS_H_

	#include "shared-sockets.h"

	typedef struct {
		int size;
		void *stream;
	} t_buffer;

	typedef struct {
		op_code operationCode;
		t_buffer *buffer;
	} t_package;

#endif
