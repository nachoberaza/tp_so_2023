#include "long-term-scheduler-utils.h"

void execute_long_term_scheduler(){

	int i = 0;

	int newProcessesListSize;
	int multiprogrammingActualLevel = get_kernel_config()->MAX_MULTIPROGRAMMING_LEVEL;
	int shortTermListSize;

	while(1){

		newProcessesListSize = list_size(get_new_pcb_list());
		shortTermListSize = list_size(get_short_term_list());

		if (should_stop_scheduling(i, newProcessesListSize, shortTermListSize, multiprogrammingActualLevel))
			break;

		// Toma el primero de la lista de new
		t_pcb *pcb = list_get(get_new_pcb_list(), 0);

		// New -> Ready
		pcb->state = READY;

		// Quita de la lista de new
		list_remove(get_new_pcb_list(), 0);

		// Lo agrega a la lista de Ready
		list_add(get_short_term_list(), pcb);

		// Decremento el grado de multiprogramacion actual
		multiprogrammingActualLevel--;

		i++;
	}

}

int should_stop_scheduling(int iteration_number,int newProcessesListSize, int shortTermListSize,int  multiprogrammingActualLevel){

	return ( iteration_number > newProcessesListSize ) 
		|| ( shortTermListSize > multiprogrammingActualLevel )
		|| ( newProcessesListSize == 0 );
}
