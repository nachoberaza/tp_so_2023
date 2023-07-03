
#ifndef UTILS_FCB_UTILS_H_
#define UTILS_FCB_UTILS_H_

	#include <shared/logs/log.h>
	#include <dirent.h>
	#include <commons/string.h>
	#include <shared/execution-context/execution-context.h>
	#include "../structs/fcb.h"
	#include "config-utils.h"

	void load_fcb_list();
	t_list* get_fcb_list();
	t_fcb* get_fcb_by_name(char* name);
	t_fcb* create_fcb_from_path(char* path);
	t_fcb* create_fcb_from_instruction(t_instruction* instruction);
	uint32_t get_direct_pointer_from_instruction(t_instruction* instruction);
	void log_fcb(t_fcb* fcb);
	void log_fcb_list(t_list* fcbList);
	char *get_fcb_path(t_instruction* instruction);
	void persist_fcb(FILE *file,t_fcb *fcb);
#endif
