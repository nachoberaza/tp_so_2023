
#ifndef PCB_UTILS_H_
#define PCB_UTILS_H_

	#include <string.h>
	#include <commons/collections/list.h>
	#include <commons/string.h>
	#include <time.h>
	#include <shared/logs/log.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/structs/execution-context.h>
	#include "../structs/pcb.h"
	#include "logger-utils.h"
	#include "config-utils.h"
	
	t_instruction* create_instruction(char* instruction);
	void start_pcb_list();
	t_pcb* new_pcb(int clientSocketId);
	t_pcb* create_pcb_from_lines(t_list* lines, int clientSocketId);
	void build_pcb(t_list *lines, int clientSocketId);
	void populate_instruction_list_from_lines(t_list* instructions,t_list* userInput);
	void add_instruction(t_pcb* pcb, t_instruction* instruction);
	void add_segment(t_pcb* pcb, t_segment_row* segment);
	void add_file(t_pcb* pcb, t_open_file_row* openFile);
	void free_pcb(t_pcb* pcb);
	void destroy_open_files_row(t_open_file_row* openFileRow);
	void destroy_segment_row(t_segment_row* segmentRow);
	int get_current_pid();
	void log_pcb(t_pcb* pcb);
	void write_segment_row_to_internal_logs(t_segment_row* segmentRow);
	void write_open_file_row_to_internal_logs(t_open_file_row* openFileRow);
	void log_pcb(t_pcb* pcb);
	t_list* get_new_pcb_list();
	t_list* get_short_term_list();
	void log_pcb_state(t_pcb* pcb);
	int pcb_is_running(t_pcb* pcb);
	int pcb_is_ready(t_pcb* pcb);
	int pcb_is_not_blocked(t_pcb* pcb);
	void move_to_ready(t_pcb* pcb);
	void move_to_blocked(t_pcb* pcb);
	void move_to_running(t_pcb* pcb);

#endif