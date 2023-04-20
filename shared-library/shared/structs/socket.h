#ifndef SHARED_STRUCTS_SOCKET_H_
#define SHARED_STRUCTS_SOCKET_H_

	typedef enum {
		MESSAGE, PACKAGE
	} op_code;

	typedef struct {
		int size;
		void *stream;
	} t_buffer;

	typedef struct {
		op_code operationCode;
		t_buffer *buffer;
	} t_package;

	typedef enum {
		KRN,
		CPU,
		FS
	} module_handshakes;


#endif
