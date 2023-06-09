#ifndef STRUCTS_PCB_H_
#define STRUCTS_PCB_H_

	#define STATE_PROCESS_ENUM_SIZE 5
	#include <shared/sockets/client-socket.h>
	#include <shared/structs/execution-context.h>

	typedef enum {
		STATE_NEW, STATE_READY, STATE_RUNNING,STATE_BLOCK,STATE_EXIT
	} state_process;

	static char * stateNames[STATE_PROCESS_ENUM_SIZE] = {
		[STATE_NEW] = "NEW",
		[STATE_READY] = "READY",
		[STATE_RUNNING] = "RUNNING",
		[STATE_BLOCK] = "BLOCK",
		[STATE_EXIT] = "EXIT",
	};

	typedef struct {
		int id;
		int baseDirection;
		int segmentSize;
	} t_segment_row;

	typedef struct {
		char* file;
		char* pointer;
	} t_open_file_row;

	typedef struct {
		int clientSocketId;
		t_list* segmentTable; //lista con elementos t_segment_row
		double nextBurstEstimate;
		int timeArrivalReady;
		t_list* openFilesTable;//lista con elementos t_open_file_row
		state_process state;
		bool firstTimeInReady;
		t_execution_context* executionContext;
	} t_pcb;

#endif
