#ifndef UTILS_MEMORY_COMMUNICATION_UTILS_H_
#define UTILS_MEMORY_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/memory/memory.h>
	#include <shared/structs/execution-context.h>
	#include <commons/collections/list.h>
	#include "config-utils.h"
	#include "socket-utils.h"
	#include "logger-utils.h"
	#include "pcb-utils.h"
	#include "thread-utils.h"
	#include "cpu-communication-utils.h"
	#include "resource-utils.h"
	#include "semaphore-utils.h"
	#include "../structs/pcb.h"


	t_list* init_process_segment_table(int pid);
	t_list* receive_process_segment_table();
	void delete_process_segment_table(int pid);
	void send_memory_data_to_memory(t_pcb* pcb);
	void execute_kernel_create_segment(t_pcb* pcb);
	void execute_kernel_delete_segment(t_pcb* pcb);
	t_memory_data* create_memory_data(t_pcb* pcb);
	void request_compaction_to_memory_and_retry(t_pcb* pcb);
	void update_segment_table_in_all_proccesses(t_list* newSegmentTable);
	t_pcb* search_pcb_by_pid(int pid);
	t_segment_row* duplicate_segment(t_segment_row* segment);
#endif
