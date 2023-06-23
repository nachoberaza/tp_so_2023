#include "socket-utils.h"

int memoryConnection;

void start_memory_connection(){
	memoryConnection = connect_to_server(get_cpu_env()->IP_MEMORY, get_cpu_env()->PORT_MEMORY);
	handle_memory_handshake(memoryConnection, CPU);
}

int get_memory_connection(){
	return memoryConnection;
}
