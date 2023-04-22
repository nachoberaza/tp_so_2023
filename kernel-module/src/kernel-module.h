#ifndef KERNEL_MODULE_H_
#define KERNEL_MODULE_H_

#define MODULE_NAME "kernel"

	typedef enum {
		NEW, READY, EXEC,BLOCK
	} state_process;

	typedef enum {
		SET,YIELD,EXIT
	} kernel_intructions;

	typedef struct {
		int id;
		int base_direction;
		int segment_size;
	} t_segment_row;

	typedef struct {
		char* file;
		char* pointer;
	} t_open_file_row;

	typedef struct {
		char AX[4];
		char BX[4];
		char CX[4] ;
		char DX[4];
		char EAX[8];
		char EBX[8];
		char ECX[8];
		char EDX[8];
		char RAX[16];
		char RBX[16];
		char RCX[16];
		char RDX[16];
	} t_cpu_register;

	#include <stdio.h>
	#include <stdlib.h>
	//#include <time.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <commons/collections/list.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include "utils/config-utils.h"
	#include "utils/socket-utils.h"
	#include "utils/logger-utils.h"

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
		kernel_intructions command;
		t_list* parameters;
	} t_kernel_instruction;


	t_pcb* new(void);
	t_pcb* create_pcb(t_list* intructions);
	t_kernel_instruction* create_instruction(char* instruction);
	t_list* create_instruction_list(t_list* userInput);
	void add_intruction(t_pcb* pcb, t_kernel_instruction* instruction);
	void add_segment(t_pcb* pcb, t_segment_row* segment);
	void add_file(t_pcb* pcb, t_open_file_row* openFile);
	void free_pcb(t_pcb* pcb);
	void write_intructions_to_all_logs(t_kernel_instruction* instruction);

#endif
