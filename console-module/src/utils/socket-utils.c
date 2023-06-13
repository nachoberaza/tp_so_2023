#include "socket-utils.h"

int kernelConnection;

void connect_to_kernel(t_console_config *consoleEnv){
	kernelConnection = connect_to_server(consoleEnv->IP_KERNEL, consoleEnv->PORT_KERNEL);

	handle_kernel_handshake();
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
		write_to_log(LOG_TARGET_ALL, LOG_LEVEL_ERROR, "[utils/socket-utils - handle_kernel_handshake] No se pudo establecer conexion con el modulo de kernel");
		exit(EXIT_FAILURE);
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[utils/socket-utils - handle_kernel_handshake] Conectado con kernel");
}

void handle_kernel_response() {
	int response;

	recv(kernelConnection, &response, sizeof(int), MSG_WAITALL);

	if(!response){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/socket-utils - handle_kernel_response] Finalizo Exitosamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/socket-utils - handle_kernel_response] Finalizo con Error");
}


void send_instructions_to_kernel(){
	t_buffer* buffer = create_buffer();
	fill_buffer_from_file(buffer);
	send_buffer(buffer, kernelConnection);
}
