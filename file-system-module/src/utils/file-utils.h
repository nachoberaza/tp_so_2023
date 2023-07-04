
#ifndef UTILS_FILE_UTILS_H_
#define UTILS_FILE_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <math.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/memory/memory.h>
 	#include "config-utils.h"
	#include "../utils/fcb-utils.h"
	#include <sys/mman.h>
	#include <fcntl.h>


	operation_result execute_fs_f_create(t_instruction* instruction);
	operation_result execute_fs_f_open(t_instruction* instruction);
	operation_result execute_fs_f_read(t_memory_data* instruction);
	operation_result execute_fs_f_write(t_memory_data* instruction);
	operation_result execute_fs_f_truncate(t_instruction* instruction);
	operation_result execute_fs_f_seek(t_instruction* instruction);
	void send_f_write_to_memory(t_memory_data* context, int size, int physicalAddress);
#endif
