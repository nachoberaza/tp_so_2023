#ifndef SHARED_STRUCTS_EXECUTION_CONTEXT_H_
#define SHARED_STRUCTS_EXECUTION_CONTEXT_H_

	#define COMMAND_ENUM_SIZE 16
	#define EXECUTION_CONTEXT_STATE_ENUM_SIZE 14
	#define ERROR_ENUM_SIZE 2
	#include <commons/collections/list.h>

	typedef enum {
		SEG_FAULT,
		OUT_OF_MEMORY
	} error;

	typedef enum {
		SET,YIELD,EXIT,
		WAIT,SIGNAL,IO,
		MOV_IN,MOV_OUT,
		F_OPEN,F_CLOSE,F_SEEK,F_READ,F_WRITE,F_TRUNCATE,
		CREATE_SEGMENT,DELETE_SEGMENT
	} command;

	static char *errorNames[ERROR_ENUM_SIZE] = {
		[SEG_FAULT] = "SEG_FAULT",
		[OUT_OF_MEMORY] = "OUT_OF_MEMORY",
	};

	static char *commandNames[COMMAND_ENUM_SIZE] = {
		[SET] = "SET",
		[YIELD] = "YIELD",
		[EXIT] = "EXIT",
		[WAIT] = "WAIT",
		[SIGNAL] = "SIGNAL",
		[IO] = "I/O",
		[MOV_IN] = "MOV_IN",
		[MOV_OUT] = "MOV_OUT",
		[F_OPEN] = "F_OPEN",
		[F_CLOSE] = "F_CLOSE",
		[F_SEEK] =  "F_SEEK",
		[F_READ] = "F_READ",
		[F_WRITE] = "F_WRITE",
		[F_TRUNCATE] = "F_TRUNCATE",
		[CREATE_SEGMENT] = "CREATE_SEGMENT",
		[DELETE_SEGMENT] = "DELETE_SEGMENT"
	};

	typedef enum {
		REASON_FINISH,
		REASON_ERROR,
		REASON_YIELD,
		REASON_WAIT,
		REASON_SIGNAL,
		REASON_IO,
		REASON_F_OPEN,
		REASON_F_CLOSE,
		REASON_F_SEEK,
		REASON_F_READ,
		REASON_F_WRITE,
		REASON_F_TRUNCATE,
		REASON_CREATE_SEGMENT,
		REASON_DELETE_SEGMENT
	} execution_context_state;

	static char *executionContextStateNames[EXECUTION_CONTEXT_STATE_ENUM_SIZE] = {
		[REASON_FINISH] = "REASON FINISH",
		[REASON_ERROR] = "REASON ERROR",
		[REASON_YIELD] = "REASON YIELD",
		[REASON_WAIT] = "REASON WAIT",
		[REASON_SIGNAL] = "REASON SIGNAL",
		[REASON_IO] = "REASON I/O",
		[REASON_F_OPEN] = "REASON F_OPEN",
		[REASON_F_CLOSE] = "REASON F_CLOSE",
		[REASON_F_SEEK] = "REASON F_SEEK",
		[REASON_F_READ] = "REASON F_READ",
		[REASON_F_WRITE] = "REASON F_WRITE",
		[REASON_F_TRUNCATE] = "REASON F_TRUNCATE",
		[REASON_CREATE_SEGMENT] = "REASON CREATE_SEGMENT",
		[REASON_DELETE_SEGMENT] = "REASON DELETE_SEGMENT"
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
