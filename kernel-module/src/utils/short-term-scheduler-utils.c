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
	// TODO: Calcular formula para cada uno
	t_pcb* nextProcess = list_get(readyList, 0);
	signal_short_term();

	return nextProcess;
}

void add_ready_time_to_processes(int time) {
	wait_short_term();
	t_list* processes = get_short_term_list();
	for(int i = 0; i < list_size(processes); i++) {
		t_pcb* process = list_get(processes, i);
		t_pcb* runningProcess = get_pcb_in_running();
		if(process->executionContext->pid != runningProcess->executionContext->pid){
			process->timeArrivalReady = time - process->timeArrivalReady;
		} else {
			runningProcess->timeArrivalReady = 0;
		}
	}
	signal_short_term();
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

