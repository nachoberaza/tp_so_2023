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
	//Get config global, check if fifo or HRRN
	t_pcb* pcb = get_first_pcb_in_ready();
	move_to_running(pcb);

	return pcb;
}

t_pcb* get_first_pcb_in_ready(){
	wait_short_term();
	t_list* listReady = list_filter(get_short_term_list(), (void *) pcb_is_ready);
	if (list_is_empty(listReady))
			return NULL;

	signal_short_term();

	return list_get(listReady, 0);
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

