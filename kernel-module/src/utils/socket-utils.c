#include "socket-utils.h"

t_kernel_connections* start_connections(t_kernel_config* env) {

	t_kernel_connections* connections = malloc(sizeof(t_kernel_connections));

	connections->cpu = connect_to_server(env->IP_CPU, env->PORT_CPU);
	connections->memory = connect_to_server(env->IP_MEMORY, env->PORT_MEMORY);
	connections->fileSystem = connect_to_server(env->IP_FILESYSTEM, env->PORT_FILESYSTEM);

	operation_result memoryHandShake = init_handshake(connections->memory, (module_handshakes) KERNEL);

	if (memoryHandShake == OPERATION_RESULT_ERROR){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "No se pudo establecer conexion con el modulo de memoria");
		exit(EXIT_FAILURE);
	}

	return connections;
}


