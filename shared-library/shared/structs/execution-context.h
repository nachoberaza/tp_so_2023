#ifndef SHARED_STRUCTS_EXECUTION_CONTEXT_H_
#define SHARED_STRUCTS_EXECUTION_CONTEXT_H_

	#define COMMAND_ENUM_SIZE 6
	#define EXECUTION_CONTEXT_STATE_ENUM_SIZE 6
	#include <commons/collections/list.h>

	typedef enum {
		SET,YIELD,EXIT,WAIT,SIGNAL,IO
	} command;

	static char *commandNames[COMMAND_ENUM_SIZE] = {
		[SET] = "SET",
		[YIELD] = "YIELD",
		[EXIT] = "EXIT",
		[WAIT] = "WAIT",
		[SIGNAL] = "SIGNAL",
		[IO] = "I/O",
	};

	typedef enum {
		REASON_FINISH,
		REASON_ERROR,
		REASON_YIELD,
		REASON_WAIT,
		REASON_SIGNAL,
		REASON_IO,
	} execution_context_state;

	static char *executionContextStateNames[EXECUTION_CONTEXT_STATE_ENUM_SIZE] = {
		[REASON_FINISH] = "REASON FINISH",
		[REASON_ERROR] = "REASON ERROR",
		[REASON_YIELD] = "REASON YIELD",
		[REASON_WAIT] = "REASON WAIT",
		[REASON_SIGNAL] = "REASON SIGNAL",
		[REASON_IO] = "REASON I/O"
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
		execution_context_state executionContextState;
		t_list* parameters;
	} t_execution_context_reason;

	typedef struct {
		int pid;
		t_list* instructions;
		int programCounter;
		t_cpu_register* cpuRegisters;
		t_execution_context_reason* reason;
	} t_execution_context;
//El exec context tmb tiene q tener la tabla de segmento, pero no sabemos como hacerla x ahora
#endif
