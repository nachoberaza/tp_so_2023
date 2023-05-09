#ifndef UTILS_STRUCTS_H_
#define UTILS_STRUCTS_H_

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
		int clientSocketId;
		int pid;
		t_list* instructions; //lista con elementos t_kernel_instructions
		int program_counter;
		t_cpu_register* cpuRegisters;
		t_list* segmentTable; //lista con elementos t_segment_row
		float nextBurstEstimate;
		int timeArrivalReady; //puede ser time_t
		t_list* openFilesTable;//lista con elementos t_open_file_row
		state_process state;
	} t_pcb;

	typedef struct {
		kernel_command command;
		t_list* parameters;
	} t_kernel_instruction;

	typedef struct {
		int id;
		int baseDirection;
		int segmentSize;
	} t_segment_row;

	typedef struct {
		char* file;
		char* pointer;
	} t_open_file_row;

#endif
