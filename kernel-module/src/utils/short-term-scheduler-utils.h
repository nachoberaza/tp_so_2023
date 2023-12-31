#ifndef UTILS_SHORT_TERM_SCHEDULER_UTILS_H_
#define UTILS_SHORT_TERM_SCHEDULER_UTILS_H_

	#include <commons/log.h>
	#include <commons/string.h>
	#include "logger-utils.h"
	#include "../structs/pcb.h"
	#include "semaphore-utils.h"
	#include "config-utils.h"
	#include "pcb-utils.h"

	t_pcb* calculate_next_process();
	t_pcb* get_next_process();
	t_pcb* get_pcb_in_running();
	void move_pcb_to_short_term_end(t_pcb* pcb);
	void remove_pid_from_short_term_list(t_pcb* pcb);
	t_pcb* get_next_process_with_fifo(t_list* readyList);
	t_pcb* get_next_process_with_hrrn(t_list* readyList);
	void recalculate_hrrn_values_to_processes(int startTime, int endTime);
	void add_aging_to_process(t_pcb* process, int startTime, int endTime);
	void reset_aging(t_pcb* process);
	void recalculate_next_burst_estimate(t_pcb* process, int lastBurst);

#endif
