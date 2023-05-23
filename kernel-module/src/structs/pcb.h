#ifndef STRUCTS_PCB_H_
#define STRUCTS_PCB_H_

	#define STATE_PROCESS_ENUM_SIZE 4
	#include <shared/sockets/client-socket.h>
	#include <shared/structs/execution-context.h>

	typedef enum {
		NEW, READY, RUNNING,BLOCK
	} state_process;

	static char * stateNames[STATE_PROCESS_ENUM_SIZE] = {
		[NEW] = "NEW",
		[READY] = "READY",
		[RUNNING] = "RUNNING",
		[BLOCK] = "BLOCK",
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
		float nextBurstEstimate;
		int timeArrivalReady;
		t_list* openFilesTable;//lista con elementos t_open_file_row
		state_process state;
		t_execution_context* executionContext;
	} t_pcb;

#endif
