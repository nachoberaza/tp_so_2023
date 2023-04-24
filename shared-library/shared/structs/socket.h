#ifndef SHARED_STRUCTS_SOCKET_H_
#define SHARED_STRUCTS_SOCKET_H_

	typedef enum {
		MESSAGE, PACKAGE
	} operation_code;

	typedef struct {
		int size;
		void *stream;
	} t_buffer;

	typedef struct {
		operation_code operationCode;
		t_buffer *buffer;
	} t_package;

	typedef enum {
		KERNEL,
		CPU,
		FILESYSTEM
	} t_module_handshakes;

	typedef enum{
		OK,
		ERROR
	}t_operation_result;

#endif
