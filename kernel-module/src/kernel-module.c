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

    placeholder_function(kernelConnections);

	pthread_join(get_listener_thread(), NULL);
	return EXIT_SUCCESS;
}

void placeholder_function(t_kernel_connections* kernelConnections) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Placeholder - Pase el thread create");

	while(1){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Sleeping 5");
		sleep(5);

		int size = list_size(get_pcb_list());
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("Cant de elementos en PCB: %d", size));

		if (size > 0){
			nosequeponerle(kernelConnections);
		}
	}
}
