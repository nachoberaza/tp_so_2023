#ifndef SHARED_STRUCTS_MEMORY_H_
#define SHARED_STRUCTS_MEMORY_H_

	typedef struct {
		int id;
		int baseDirection;
		int segmentSize;
	} t_segment_row;

	typedef enum {
		CREATE_SEGMENT_TABLE,
		CREATE_SEGMENT_KERNEL,
		DELETE_SEGMENT_KERNEL
	} t_memory_command;

	typedef enum {
		SEGMENT_TABLE_CREATED
	} t_kernel_command;

#endif
