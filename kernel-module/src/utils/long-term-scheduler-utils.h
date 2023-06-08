#ifndef UTILS_LONG_TERM_SCHEDULER_UTILS_H_
#define UTILS_LONG_TERM_SCHEDULER_UTILS_H_

	#include "config-utils.h"
	#include "pcb-utils.h"

	void execute_long_term_scheduler();
	int should_stop_scheduling( int newProcessesListSize, int shortTermListSize, int shortTermProcessesAvailables );
	void move_to_exit(t_pcb* pcb);

#endif
