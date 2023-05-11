#include "cpu-communication-utils.h"

void send_context_to_cpu(t_execution_context *commands, t_kernel_connections* KERNEL_CONNECTIONS) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Before sending");
	t_package* package = create_package();

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Package created");
	fill_package_with_context(get_logger(), commands, package);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Filled with context");

	send_package(package, KERNEL_CONNECTIONS->cpu);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Send package");
}

t_execution_context* listen_cpu_response(int cpuSocket) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Ejecutando handle_cpu_response");
	receive_operation_code(cpuSocket);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Respuesta recibida del CPU");
	t_execution_context* context = decode_context(get_logger(), LOG_LEVEL_TRACE, cpuSocket);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Respuesta decodificada del CPU");
	log_context(get_logger(), LOG_LEVEL_TRACE, context);

	return context;
}

void handle_cpu_response(t_pcb* pcb){
	switch(pcb->executionContext->exitReason){
		case REASON_BLOCK:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Termino instrucción, moviendo a blocked");
			list_remove(get_pcb_list(), 0);
			list_add(get_pcb_list(), pcb);
			break;
		case REASON_FINISH:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Done, eliminando PCB");
			list_remove(get_pcb_list(), 0);
			free_pcb(pcb);
			break;
		case REASON_RUNNING:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "REASON_RUNNING");
			break;
		case REASON_ERROR:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Error en la ejecucion. Termino programa");
			exit(EXIT_FAILURE);
			break;
	}
}

void nosequeponerle(t_kernel_connections* kernelConnections){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Sending context");

	t_pcb* pcb = list_get(get_pcb_list(), 0);

	log_context(get_logger(), LOG_LEVEL_DEBUG, pcb->executionContext);

	send_context_to_cpu(pcb->executionContext, kernelConnections);

	t_execution_context* responseContext= listen_cpu_response(kernelConnections->cpu);

	destroy_execution_context(get_logger(), pcb->executionContext);
	pcb->executionContext = responseContext;

	handle_cpu_response(pcb);
}
