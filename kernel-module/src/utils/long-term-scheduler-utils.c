#include "long-term-scheduler-utils.h"

void execute_long_term_scheduler(){

	int newProcessesListSize, shortTermProcessesAvailables, shortTermListSize;

	while(1){

		newProcessesListSize = list_size(get_new_pcb_list());
		shortTermListSize = list_size(get_short_term_list());
		shortTermProcessesAvailables = get_kernel_config()->MAX_MULTIPROGRAMMING_LEVEL - shortTermListSize;

		if (should_stop_scheduling(newProcessesListSize, shortTermListSize, shortTermProcessesAvailables))
			break;

		t_pcb *pcb = list_get(get_new_pcb_list(), 0);

		pcb->state = READY;

		list_add(get_short_term_list(), pcb);

		list_remove(get_new_pcb_list(), 0);
	}

}

int should_stop_scheduling(int newProcessesListSize, int shortTermListSize,int  shortTermProcessesAvailables){

	return ( shortTermProcessesAvailables == 0 )
		|| ( newProcessesListSize == 0 );
}
