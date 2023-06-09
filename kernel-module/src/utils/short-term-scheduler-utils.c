#include "short-term-scheduler-utils.h"

t_pcb* calculate_next_process (){
	t_pcb* running = get_pcb_in_running();
	if (running != NULL)
		return running;


	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_DEBUG,
			string_from_format("[utils/cpu-communication-utils - calculate_next_process] Eligiendo próximo proceso ")
	);

	t_pcb* pcb = get_next_process();
	move_to_running(pcb);

	return pcb;
}

t_pcb* get_next_process(){
	wait_short_term();
	t_list* readyList = list_filter(get_short_term_list(), (void *) pcb_is_ready);
	if (list_is_empty(readyList))
			return NULL;

	signal_short_term();

	if(get_planning_algorithm() == HRRN) {
		return get_next_process_with_hrrn(readyList);
	}

	return get_next_process_with_fifo(readyList);
}

t_pcb* get_next_process_with_fifo(t_list* readyList) {
	wait_short_term();
		t_pcb* nextProcess = list_get(readyList, 0);
	signal_short_term();

	return nextProcess;
}

t_pcb* get_next_process_with_hrrn(t_list* readyList) {
	wait_short_term();
	t_pcb* nextProcess = list_get(readyList, 0);
	double responseRatio = (nextProcess->timeArrivalReady + nextProcess->nextBurstEstimate) / nextProcess->nextBurstEstimate;

	for(int i = 0; i < list_size(readyList); i++) {
		t_pcb* process = list_get(readyList, i);
		double processResponseRatio = (process->timeArrivalReady + process->nextBurstEstimate) / process->nextBurstEstimate;
		if(processResponseRatio > responseRatio) {
			nextProcess = process;
			responseRatio = processResponseRatio;
		}
		write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_INFO,
			string_from_format("[utils/cpu-communication-utils - get_next_process_with_hrrn] PID: %d - timeArrivalReady: %d - nextBurstEstimate: %f - processResponseRatio: %f", process->executionContext->pid, process->timeArrivalReady, process->nextBurstEstimate, processResponseRatio)
		);
	}
	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_INFO,
		string_from_format("------------------------------------------------------------------")
	);
	signal_short_term();

	return nextProcess;
}

void recalculate_hrrn_values_to_processes(int startTime, int endTime) {
	wait_short_term();
	t_list* processes = get_short_term_list();
	for(int i = 0; i < list_size(processes); i++) {
		t_pcb* process = list_get(processes, i);
		if(process->state == STATE_READY){
			add_aging_to_process(process, startTime, endTime);
		} else {
			reset_aging(process);
			// TODO: Chequear si solo se tiene que hacer aca o si se tiene que hacer tambien cuando pasa de blocked a ready
			if(process->state == STATE_RUNNING)
				recalculate_next_burst_estimate(process, endTime - startTime);
		}
	}
	signal_short_term();
}

void add_aging_to_process(t_pcb* process, int startTime, int endTime) {
	if(process->firstTimeInReady){
		process->timeArrivalReady = endTime - process->timeArrivalReady;
	} else {
		process->timeArrivalReady = process->timeArrivalReady + endTime - startTime;
	}
	process->firstTimeInReady = false;
}

void reset_aging(t_pcb* process) {
	process->timeArrivalReady = 0;
	process->firstTimeInReady = false;
}

void recalculate_next_burst_estimate(t_pcb* process, int lastBurst) {
	double alphaValue = get_kernel_config()->HRRN_ALFA;
	process->nextBurstEstimate = (alphaValue * process->nextBurstEstimate) + ((1 - alphaValue) * lastBurst);
}

t_pcb* get_pcb_in_running(){
	wait_short_term();
	t_list* listRunning = list_filter(get_short_term_list(), (void *) pcb_is_running);
	signal_short_term();
	if (list_is_empty(listRunning))
		return NULL;

	return list_get(listRunning, 0);
}

void move_pcb_to_short_term_end(t_pcb* pcb){
	write_to_log(
						LOG_TARGET_INTERNAL,
						LOG_LEVEL_DEBUG,
						string_from_format("[utils/cpu-communication-utils - move_pcb_to_short_term_end] Moviendo al final PID: %d", pcb->executionContext->pid)
				);

	wait_short_term();
	move_to_ready(pcb);
	remove_pid_from_short_term_list(pcb);
	list_add(get_short_term_list(), pcb);
	signal_short_term();
}

void remove_pid_from_short_term_list(t_pcb* pcb){
	write_to_log(
					LOG_TARGET_INTERNAL,
					LOG_LEVEL_DEBUG,
					string_from_format("[utils/cpu-communication-utils - remove_pid_from_short_term_list] Removiendo PID: %d", pcb->executionContext->pid)
			);

	t_list* shortTermList = get_short_term_list();
	int size = list_size(shortTermList);

	for (int i=0; i<size; i++){
	t_pcb* listPcb = list_get(shortTermList, i);
		if (pcb->executionContext->pid == listPcb->executionContext->pid)
		{
			list_remove(shortTermList, i);
			return;
		}
	}
}

