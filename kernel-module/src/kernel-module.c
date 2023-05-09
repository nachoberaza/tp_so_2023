#include "kernel-module.h"

int main(void) {
	t_kernel_config *kernelEnv = create_kernel_config(MODULE_NAME);

	init_logger(MODULE_NAME, kernelEnv->LOG_LEVEL);
	log_config(kernelEnv);

	int serverSocketId = start_server(kernelEnv->IP, kernelEnv->PORT, get_logger());
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Servidor de kernel listo");

	t_kernel_connections* kernelConnections = start_connections(kernelEnv);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Kernel conectado a CPU, FileSystem y Memoria");

	create_listener_thread(serverSocketId);

	// TODO: Reemplazar funcion con la funcion que haga el procesamiento.
    placeholder_function();

	pthread_join(get_listener_thread(), NULL);
	return EXIT_SUCCESS;
}

void placeholder_function() {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Placeholder - Pase el thread create");
}
