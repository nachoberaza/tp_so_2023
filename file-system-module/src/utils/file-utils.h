
#ifndef UTILS_FILE_UTILS_H_
#define UTILS_FILE_UTILS_H_
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <shared/execution-context/execution-context.h>
 	#include "config-utils.h"
	#include "../utils/fcb-utils.h"


	operation_result execute_fs_f_create(t_instruction* instruction);
	operation_result execute_fs_f_open(t_instruction* instruction);
	operation_result execute_fs_f_read(t_instruction* instruction);
	operation_result execute_fs_f_write(t_instruction* instruction);
	operation_result execute_fs_f_truncate(t_instruction* instruction);
	operation_result execute_fs_f_seek(t_instruction* instruction);
#endif
