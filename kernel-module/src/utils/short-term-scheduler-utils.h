#ifndef UTILS_SHORT_TERM_SCHEDULER_UTILS_H_
#define UTILS_SHORT_TERM_SCHEDULER_UTILS_H_

	#include <commons/log.h>
	#include <commons/string.h>
	#include "logger-utils.h"
	#include "../structs/pcb.h"
	#include "semaphore-utils.h"
	#include "pcb-utils.h"

	t_pcb* calculate_next_process();
	t_pcb* get_first_pcb_in_ready();
	t_pcb* get_pcb_in_running();
	void move_pcb_to_short_term_end(t_pcb* pcb);
	void remove_pid_from_short_term_list(t_pcb* pcb);

#endif
