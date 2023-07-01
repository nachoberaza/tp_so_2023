#include "long-term-scheduler-utils.h"

void execute_long_term_scheduler(){

	int newProcessesListSize, shortTermProcessesAvailables, shortTermListSize;

	while(1){

		newProcessesListSize = list_size(get_new_pcb_list());
		shortTermListSize = list_size(get_short_term_list());
		shortTermProcessesAvailables = get_kernel_config()->MAX_MULTIPROGRAMMING_LEVEL - shortTermListSize;

		if (should_stop_scheduling(newProcessesListSize, shortTermProcessesAvailables))
			break;

		t_pcb *pcb = list_get(get_new_pcb_list(), 0);

		pcb->executionContext->segmentTable = init_process_segment_table(pcb->executionContext->pid);

		log_pcb(pcb);

		write_to_log(
			LOG_TARGET_MAIN,
			LOG_LEVEL_INFO,
			string_from_format(
			"Cambio de Estado: PID: %d - Estado Anterior: %s - Estado Actual: %s",
			pcb->executionContext->pid,state_as_string(pcb->state),state_as_string(STATE_READY))
		);

		pcb->state = STATE_READY;

		wait_short_term();
		list_add(get_short_term_list(), pcb);

		//REVISAR SI VA ACA ESTE LOG
		write_to_log(
			LOG_TARGET_MAIN,
			LOG_LEVEL_INFO,
			string_from_format("Cola Ready %s: [%s]",planningAlgorithmNames[get_planning_algorithm()], get_string_array_pid(get_short_term_list()))
		);

		signal_short_term();

		list_remove(get_new_pcb_list(), 0);
	}

}

int should_stop_scheduling(int newProcessesListSize, int shortTermProcessesAvailables){
	return (shortTermProcessesAvailables == 0 ) || ( newProcessesListSize == 0 );
}

void move_to_exit(t_pcb* pcb){
	operation_result result = OPERATION_RESULT_OK;
	char* reason = "Success";

	if (pcb->executionContext->reason->executionContextState == REASON_ERROR && !list_is_empty(pcb->executionContext->reason->parameters)){
		reason = list_get(pcb->executionContext->reason->parameters,0);
		result = OPERATION_RESULT_ERROR;
	}

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("Cambio de Estado: “PID: %d - Estado Anterior: %s - Estado Actual: %s",
		pcb->executionContext->pid, state_as_string(pcb->state), state_as_string(STATE_EXIT))
	);

	write_to_log(
			LOG_TARGET_MAIN,
			LOG_LEVEL_INFO,
			string_from_format("Finaliza el proceso %d - Motivo: %s", pcb->executionContext->pid,reason)
	);


	delete_process_segment_table(pcb->executionContext->pid);

	wait_short_term();
	remove_pid_from_short_term_list(pcb);
	signal_short_term();

	send(pcb->clientSocketId, &result, sizeof(operation_result), 0);

	free_pcb(pcb);


	execute_long_term_scheduler();
}
