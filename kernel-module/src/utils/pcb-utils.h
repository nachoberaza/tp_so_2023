
#ifndef PCB_UTILS_H_
#define PCB_UTILS_H_

	#include <commons/collections/list.h>
	#include <shared/logs/log.h>
	#include "logger-utils.h"
	#include "structs.h"

	t_pcb* new_pcb(void);
	t_pcb* create_pcb_from_lines(t_list* intructions);
	t_kernel_instruction* create_instruction(char* instruction);
	void populate_instruction_list_from_lines(t_list* instructions,t_list* userInput);
	void add_instruction(t_pcb* pcb, t_kernel_instruction* instruction);
	void add_segment(t_pcb* pcb, t_segment_row* segment);
	void add_file(t_pcb* pcb, t_open_file_row* openFile);
	void free_pcb(t_pcb* pcb);
	void write_intructions_to_all_logs(t_kernel_instruction* instruction);
	kernel_command kernel_command_from_string(char * command);
	char * kernel_command_as_string(kernel_command command);
	void destroy_open_files_row(t_open_file_row* openFileRow);
	void destroy_kernel_instruction(t_kernel_instruction* instruction);
	void destroy_segment_row(t_segment_row* segmentRow);
	int get_pid();
	void write_segment_row_to_internal_logs(t_segment_row* segmentRow);
	void write_open_file_row_to_internal_logs(t_open_file_row* openFileRow);
	void write_instruction_to_internal_logs(t_kernel_instruction* instruction);

#endif
