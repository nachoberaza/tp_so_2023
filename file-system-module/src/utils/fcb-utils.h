
#ifndef UTILS_FCB_UTILS_H_
#define UTILS_FCB_UTILS_H_

	#include <shared/logs/log.h>
	#include <shared/execution-context/execution-context.h>
	#include "../structs/fcb.h"
	#include "config-utils.h"

	t_fcb* create_fcb_from_instruction(t_instruction* instruction);
	uint32_t get_direct_pointer_from_instruction(t_instruction* instruction);
	void log_fcb(t_fcb* fcb);
	char *get_fcb_path(t_instruction* instruction);
	void persist_fcb(FILE *file,t_fcb *fcb);
#endif
