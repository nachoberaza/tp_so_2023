#include "cpu-communication-utils.h"

void handle_processes() {
	while(1){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[utils/cpu-communication-utils - handle_processes] Sleeping 5");
		sleep(5);

		int size = list_size(list_filter(get_short_term_list(), (void *) pcb_is_not_blocked));
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("[utils/cpu-communication-utils - handle_processes] Cantidad de procesos: %d", size));

		list_iterate(get_short_term_list(), (void *) log_pcb_state);

		if (size > 0){
			execute_process();
		}
	}
}

void execute_process(){
	t_pcb* pcb = calculate_next_process();
	int startTime, finishTime;
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_DEBUG,
			string_from_format("[utils/cpu-communication-utils - execute_process] Ejecutando PID: %d", pcb->executionContext->pid)
	);

	log_context(get_logger(), LOG_LEVEL_TRACE, pcb->executionContext);

	send_context_to_cpu(pcb->executionContext);
	startTime = (int)time(NULL);
	t_execution_context* responseContext = listen_cpu_response();
	finishTime = (int)time(NULL);
	destroy_execution_context(get_logger(), pcb->executionContext);
	pcb->executionContext = responseContext;
	if(get_planning_algorithm() == HRRN) {
		recalculate_hrrn_values_to_processes(startTime, finishTime);
	}
	handle_cpu_response(pcb);
}


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
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_INFO,
			string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Reason: %d", pcb->executionContext->reason->executionContextState)
	);
	switch(pcb->executionContext->reason->executionContextState){
		case REASON_YIELD:{
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Cambio de status en el PID: %d - Blocked", pcb->executionContext->pid)
			);
			move_pcb_to_short_term_end(pcb);
			break;
		}
		case REASON_FINISH:{
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Cambio de status en el PID: %d - Finish", pcb->executionContext->pid)
			);

			move_to_exit(pcb);

			break;
		}
		case REASON_ERROR:{
			write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - handle_cpu_response] Error en el PID: %d", pcb->executionContext->pid)
			);

			move_to_exit(pcb);

			break;
		}
		case REASON_WAIT:{
			execute_kernel_wait(pcb);
			break;
		}
		case REASON_SIGNAL:{
			execute_kernel_signal(pcb);
			break;
		}
		case REASON_IO:{
			execute_kernel_io(pcb);
			break;
		}
		case REASON_CREATE_SEGMENT:{
			execute_kernel_create_segment(pcb);
			break;
		}
		case REASON_DELETE_SEGMENT:{
			execute_kernel_delete_segment(pcb);
			break;
		}
		case REASON_F_OPEN:{
			execute_kernel_f_open(pcb);
			break;
		}
		case REASON_F_CLOSE:{
			execute_kernel_f_close(pcb);
			break;
		}
		case REASON_F_SEEK:{
			execute_kernel_f_seek(pcb);
		break;
		}
		case REASON_F_READ:{
			execute_kernel_f_read(pcb);
			break;
		}
		case REASON_F_WRITE:{
			execute_kernel_f_write(pcb);
			break;
		}
		case REASON_F_TRUNCATE:{
			execute_kernel_f_truncate(pcb);
			break;
		}
	}
}





void execute_kernel_wait(t_pcb* pcb){
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_INFO,
			string_from_format("[utils/cpu-communication-utils - execute_kernel_wait] ejecutando wait en el PID: %d", pcb->executionContext->pid)
	);
	char* resourceName = list_get(pcb->executionContext->reason->parameters, 0);

	t_resource* resource = get_resource(get_resources_list(),resourceName);

	check_null_resource(resource);

	if(resource->instances <= 0){
		write_to_log(
			LOG_TARGET_MAIN,
			LOG_LEVEL_INFO,
			string_from_format("PID: %d - Bloqueado por: %s", pcb->executionContext->pid,resourceName)
		);
		move_to_blocked(pcb);
		list_add(resource->blocked, pcb);
	}

	resource->instances--;

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d - Wait: %s - Instancias: %d", pcb->executionContext->pid, resourceName, resource->instances)
	);
}

void execute_kernel_signal(t_pcb* pcb){
	char* resourceName = list_get(pcb->executionContext->reason->parameters, 0);

	t_resource* resource= get_resource(get_resources_list(),resourceName);

	check_null_resource(resource);

	resource->instances++;

	if(!list_is_empty(resource->blocked)){
		t_pcb* processBloqued = list_get(resource->blocked, 0);
		list_remove(resource->blocked, 0);
		move_to_ready(processBloqued);
	}

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d - Signal: %s - Instancias: %d", pcb->executionContext->pid, resourceName, resource->instances)
	);
}

void execute_kernel_io(t_pcb* pcb){
	int time = atoi(list_get(pcb->executionContext->reason->parameters, 0));

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d - Ejecuta IO: %d", pcb->executionContext->pid,time)
	);

	t_io_thread_args* args = malloc(sizeof(t_io_thread_args));

	args->timeIO = time;
	args->pcb= pcb;

	move_to_blocked(pcb);

	pthread_t ioThread;
	int threadStatus = pthread_create(&ioThread, NULL, sleepThread, (void*)args);
	if (threadStatus != 0){
	   write_to_log(
	    	LOG_TARGET_INTERNAL,
			LOG_LEVEL_INFO,
			string_from_format("[utils/cpu-communication-utils - execute_kernel_io] Hubo un problema al crear el ioThread - Reason: %s", strerror(threadStatus))
	   );
	   exit(EXIT_FAILURE);
	}
}

void* sleepThread(void* args){
	t_io_thread_args* argsStruct =(t_io_thread_args*)args;

	sleep(argsStruct->timeIO);
	write_to_log(
		    	LOG_TARGET_INTERNAL,
				LOG_LEVEL_INFO,
				string_from_format("[utils/cpu-communication-utils - sleepThread] Finalizó I/O, agregando proceso: %d", argsStruct->pcb->executionContext->pid)
		   );
	move_pcb_to_short_term_end(argsStruct->pcb);

	return 1;
}

