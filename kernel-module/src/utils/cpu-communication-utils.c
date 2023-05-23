#include "cpu-communication-utils.h"

void handle_processes() {
	while(1){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[utils/cpu-communication-utils - handle_processes] Sleeping 5");
		sleep(5);

		int size = list_size(get_pcb_list());
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("[utils/cpu-communication-utils - handle_processes] Cantidad de procesos: %d", size));

		if (size > 0){
			execute_process();
		}
	}
}

void execute_process(){
	t_pcb* pcb = list_get(get_pcb_list(), 0);

	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_DEBUG,
			string_from_format("[utils/cpu-communication-utils - execute_process] Ejecutando PID: %d", pcb->executionContext->pid)
	);

	log_context(get_logger(), LOG_LEVEL_DEBUG, pcb->executionContext);

	send_context_to_cpu(pcb->executionContext);

	t_execution_context* responseContext = listen_cpu_response();

	destroy_execution_context(get_logger(), pcb->executionContext);
	pcb->executionContext = responseContext;

	handle_cpu_response(pcb);
}

//TODO: Cambiarlo a solo buffer
void send_context_to_cpu(t_execution_context *commands) {
	t_package* package = create_package();

	fill_package_with_context(get_logger(), commands, package);

	send_package(package, get_cpu_connection());
}

t_execution_context* listen_cpu_response() {
	receive_operation_code(get_cpu_connection());

	t_execution_context* context = decode_context(get_logger(), LOG_LEVEL_TRACE, get_cpu_connection());

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[utils/cpu-communication-utils - listen_cpu_response] Contexto creado:");
	log_context(get_logger(), LOG_LEVEL_TRACE, context);

	return context;
}

void handle_cpu_response(t_pcb* pcb){
	switch(pcb->executionContext->exitReason){
		case REASON_BLOCK:
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Cambio de status en el PID: %d - Blocked", pcb->executionContext->pid)
			);
			list_remove(get_pcb_list(), 0);
			list_add(get_pcb_list(), pcb);
			break;
		case REASON_FINISH:
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Cambio de status en el PID: %d - Finish", pcb->executionContext->pid)
			);
			list_remove(get_pcb_list(), 0);
			free_pcb(pcb);
			break;
		case REASON_RUNNING:
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Cambio de status en el PID: %d - Running", pcb->executionContext->pid)
			);
			break;
		case REASON_ERROR:
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Error en el PID: %d", pcb->executionContext->pid)
			);
			exit(EXIT_FAILURE);
			break;
	}
}


