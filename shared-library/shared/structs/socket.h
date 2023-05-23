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
	} module_handshakes;

	typedef enum{
		OPERATION_RESULT_OK,
		OPERATION_RESULT_ERROR
	}operation_result;

#endif
