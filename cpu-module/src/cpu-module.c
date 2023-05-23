#include "cpu-module.h"


int main(void) {
	t_cpu_config *CPU_ENV = create_cpu_config(MODULE_NAME);

	init_logger(MODULE_NAME, CPU_ENV->LOG_LEVEL);

	log_config(CPU_ENV);

	int memoryConnection = connect_to_server(CPU_ENV->IP_MEMORY, CPU_ENV->PORT_MEMORY);
	handle_memory_handshake(memoryConnection, CPU);

	int serverSocketId = start_server(CPU_ENV->IP, CPU_ENV->PORT, get_logger(), MODULE_NAME);

	int kernelConnection = await_client(get_logger(), serverSocketId);

	t_execution_context *context;
	//Matar variables post serializado
	//Usar buffer en vez de package

	while (1) {
		receive_operation_code(kernelConnection);

		context = decode_context(get_logger(), LOG_LEVEL_DEBUG, kernelConnection);

		log_context(get_logger(), LOG_LEVEL_DEBUG, context);

		execute_execution_context(context);

		return_execution_context(context, kernelConnection);

		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[cpu-module - main] Sleeping 5");
		sleep(5);
	}

	return EXIT_SUCCESS;
}





