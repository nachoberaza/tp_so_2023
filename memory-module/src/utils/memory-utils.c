#include "memory-utils.h"

void* memory;
t_list* freeSpacesList;

void create_memory_structures() {
	memory = malloc(get_memory_config()->MEMORY_SIZE);
}

t_list* create_segment_table() {
	t_list* segmentTable = list_create();

	t_segment_row* segmentZero = malloc(sizeof(t_segment_row));
	segmentZero->id = 0;
	segmentZero->baseDirection = 0;
	segmentZero->segmentSize = get_memory_config()->SEGMENT_ZERO_SIZE;

	list_add(segmentTable, segmentZero);
	return segmentTable;
}
