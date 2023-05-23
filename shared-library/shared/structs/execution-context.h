#ifndef SHARED_STRUCTS_EXECUTION_CONTEXT_H_
#define SHARED_STRUCTS_EXECUTION_CONTEXT_H_

	#define COMMAND_ENUM_SIZE 3
	#define EXIT_REASON_ENUM_SIZE 4
	#include <commons/collections/list.h>

	typedef enum {
				SET,YIELD,EXIT
	} command;

	static char *commandNames[COMMAND_ENUM_SIZE] = {
		[SET] = "SET",
		[YIELD] = "YIELD",
		[EXIT] = "EXIT",
	};

	typedef enum {
		REASON_FINISH,
		REASON_ERROR,
		REASON_BLOCK,
		REASON_RUNNING
	} exit_reason;

	static char *exitReasonNames[EXIT_REASON_ENUM_SIZE] = {
		[REASON_FINISH] = "REASON FINISH",
		[REASON_ERROR] = "REASON ERROR",
		[REASON_BLOCK] = "REASON BLOCK",
		[REASON_RUNNING] = "REASON RUNNING"
	};

	typedef struct {
			char AX[5];
			char BX[5];
			char CX[5];
			char DX[5];
			char EAX[9];
			char EBX[9];
			char ECX[9];
			char EDX[9];
			char RAX[17];
			char RBX[17];
			char RCX[17];
			char RDX[17];
	}t_cpu_register;

	typedef struct {
		command command;
		t_list* parameters;
	} t_instruction;

	typedef struct {
		int pid;
		t_list* instructions;
		int programCounter;
		t_cpu_register* cpuRegisters;
		exit_reason exitReason;
	} t_execution_context;
//El exec context tmb tiene q tener la tabla de segmento, pero no sabemos como hacerla x ahora
#endif
