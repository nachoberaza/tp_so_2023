#ifndef SHARED_STRUCTS_MEMORY_H_
#define SHARED_STRUCTS_MEMORY_H_

	typedef struct {
		int pid;
		int id;
		int baseDirection;
		int segmentSize;
	} t_segment_row;

	typedef enum {
		OUT_OF_MEMORY = -1,
		NEED_COMPACTION = -2
	} t_memory_add_response;

#endif
