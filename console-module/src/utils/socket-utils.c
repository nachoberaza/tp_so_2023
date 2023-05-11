#include "socket-utils.h"

int kernelConnection;

void connect_to_kernel(t_console_config *consoleEnv){
	kernelConnection = connect_to_server(consoleEnv->IP_KERNEL, consoleEnv->PORT_KERNEL);
}

int get_kernel_connection(){
	return kernelConnection;
}

void handle_kernel_handshake() {
	const int KERNEL_HANDSHAKE = 0;
	int handshakeStatus;

	send(kernelConnection, &KERNEL_HANDSHAKE, sizeof(int), 0);

	recv(kernelConnection, &handshakeStatus, sizeof(int), MSG_WAITALL);

	if(handshakeStatus != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "No se pudo conectar con kernel");
		exit(EXIT_FAILURE);
	}
}

void handle_kernel_response() {
	int response;

	recv(kernelConnection, &response, sizeof(int), MSG_WAITALL);
}


void send_instructions_to_kernel(){
	t_buffer* buffer = create_buffer();
	fill_buffer_from_file(buffer);
	send_buffer(buffer, kernelConnection);
}
