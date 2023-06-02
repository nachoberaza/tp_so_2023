#include "long-term-scheduler-utils.h"

void execute_long_term_scheduler(){

	int new_processes_list_size = list_size(get_new_pcb_list());
	int max_multiprogramming_level = get_kernel_config()->MAX_MULTIPROGRAMMING_LEVEL;
	int short_term_list_size = list_size(get_short_term_list());

	for (int i=0;
		(i<new_processes_list_size) && (short_term_list_size <= max_multiprogramming_level);
		i++
	){
		t_pcb* pcb = list_get(get_new_pcb_list(), 0);
		pcb->state = READY;
		list_add(get_short_term_list(), pcb);
	}

	list_clean(get_new_pcb_list());
}
