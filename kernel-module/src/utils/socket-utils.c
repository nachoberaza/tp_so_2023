#include "socket-utils.h"

t_kernel_connections* start_connections(t_kernel_config* env) {
	t_kernel_connections* connections = malloc(sizeof(t_kernel_connections));

	connections->cpu = connect_to_server(env->IP_CPU, env->PORT_CPU);
	connections->memory = connect_to_server(env->IP_MEMORY, env->PORT_MEMORY);
	connections->fileSystem = connect_to_server(env->IP_FILESYSTEM, env->PORT_FILESYSTEM);

	int memoryHandShake = init_handshake(connections->memory, KERNEL);
	if (memoryHandShake == ERROR){
		return -1;
	}
	return connections;
}


