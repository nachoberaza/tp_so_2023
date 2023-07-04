
#ifndef UTILS_FILE_SYSTEM_COMMUNICATION_UTILS_H_
#define UTILS_FILE_SYSTEM_COMMUNICATION_UTILS_H_
	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <time.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/structs/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include <shared/execution-context/execution-context.h>
	#include "config-utils.h"
	#include "socket-utils.h"
	#include "logger-utils.h"
	#include "pcb-utils.h"
	#include "thread-utils.h"
	#include "resource-utils.h"
	#include "../structs/pcb.h"
	#include "../structs/resource.h"
	#include "../structs/config.h"
	#include "semaphore-utils.h"
	#include "short-term-scheduler-utils.h"
	#include "memory-communication-utils.h"
	#include "file-system-communication-utils.h"

	void start_open_files_table_list();
	t_list* get_open_files_table_list();
	t_open_file_row* new_open_file_row(char *fileName);
    void execute_kernel_f_open(t_pcb* pcb);
    void execute_kernel_f_close(t_pcb* pcb);
	void execute_kernel_f_write(t_pcb* pcb);
	void execute_kernel_f_seek(t_pcb* pcb);
	void execute_kernel_f_read(t_pcb* pcb);
	void execute_kernel_f_close(t_pcb* pcb);
	void execute_kernel_f_truncate(t_pcb* pcb);
	t_instruction* duplicate_instruction(t_instruction* currentInstruction);
    int open_files_table_contains(char *fileName);
    void add_file_to_open_files_table(t_pcb* pcb,char *fileName);
    void add_file_to_process_open_files_table(t_pcb* pcb, t_instruction* instruction);
    void send_instruction_to_fs(t_instruction* instruction, int pid);
    void send_current_instruction_to_fs(t_pcb* pcb);
    void remove_file_from_process_open_files_table(t_pcb* pcb, t_instruction* instruction);
    void remove_file_from_open_files_table(t_resource* resource);
    void request_file(t_resource* resource, t_pcb* pcb);
    int get_open_file_index(t_list * list,char* fileName);
    void await_f_write(t_pcb* pcb);

#endif
