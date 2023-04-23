
#ifndef PCB_UTILS_H_
#define PCB_UTILS_H_

	#include <commons/collections/list.h>
	#include <shared/logs/log.h>
	#include "logger-utils.h"

	#define COMMAND_ENUM_SIZE 3
	#define STATE_PROCESS_ENUM_SIZE 4

	typedef enum {
		NEW, READY, EXEC,BLOCK
	} state_process;

	typedef enum {
			SET,YIELD,EXIT
	} kernel_command;

	static char * commandNames[COMMAND_ENUM_SIZE] = {
		[SET] = "SET",
		[YIELD] = "YIELD",
		[EXIT] = "EXIT",
	};

	static char * stateNames[STATE_PROCESS_ENUM_SIZE] = {
		[NEW] = "NEW",
		[READY] = "READY",
		[EXEC] = "EXEC",
		[BLOCK] = "BLOCK",
	};

	typedef struct {
			char AX[4];
			char BX[4];
			char CX[4];
			char DX[4];
			char EAX[8];
			char EBX[8];
			char ECX[8];
			char EDX[8];
			char RAX[16];
			char RBX[16];
			char RCX[16];
			char RDX[16];
	}t_cpu_register;


	typedef struct {
		int pid;
		t_list* instructions; //lista con elementos t_kernel_instructions
		int program_counter;
		t_cpu_register* CPU_registers;
		t_list* segment_table; //lista con elementos t_segment_row
		float next_burst_estimate;
		int time_arrival_ready; //puede ser time_t
		t_list* open_files_table;//lista con elementos t_open_file_row
		state_process state;
	} t_pcb;

	typedef struct {
		kernel_command command;
		t_list* parameters;
	} t_kernel_instruction;

	typedef struct {
		int id;
		int base_direction;
		int segment_size;
	} t_segment_row;

	typedef struct {
		char* file;
		char* pointer;
	} t_open_file_row;

	t_pcb* new_pcb(void);
	t_pcb* create_pcb_from_lines(t_list* intructions);
	t_kernel_instruction* create_instruction(char* instruction);
	t_list* create_instruction_list_from_lines(t_list* userInput);
	void add_intruction(t_pcb* pcb, t_kernel_instruction* instruction);
	void add_segment(t_pcb* pcb, t_segment_row* segment);
	void add_file(t_pcb* pcb, t_open_file_row* openFile);
	void free_pcb(t_pcb* pcb);
	void write_intructions_to_all_logs(t_kernel_instruction* instruction);
	kernel_command kernel_command_from_string(char * command);
	char * kernel_command_as_string(kernel_command command);
	void destroy_open_files_table(t_open_file_row* openFileRow);
	void destroy_kernel_instructions(t_kernel_instruction* instruction);
	void destroy_segment_table(t_segment_row* segmentRow);
	int get_pid();
	void write_segment_row_to_internal_logs(t_segment_row* segmentRow);
	void write_open_file_row_to_internal_logs(t_open_file_row* openFileRow);
	void write_instruction_to_internal_logs(t_kernel_instruction* instruction);

#endif
