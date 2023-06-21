#include "memory-utils.h"

void* memory;
t_list* freeSpacesList;
// TODO: Chequear nombre, den recomendaciones
t_list* segmentTableGlobal;

void create_memory_structures() {
	memory = malloc(get_memory_config()->MEMORY_SIZE);
	freeSpacesList = list_create();
	segmentTableGlobal = list_create();
}

void* get_memory() {
	return memory;
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

/*
 * TODO: Chequear si hay que fijarse primero en freeSpacesList antes de hacer malloc para newSegment
 * Ademas, hay que asignar la baseDirection segun el algoritmo
 * Primer parametro de segmentParameters es segmentId, segundo parametro es segmentSize
*/
void execute_create_segment(int kernelConnection) {
	t_list* segmentParameters = decode_segment_parameters(kernelConnection);
	t_segment_row* newSegment = malloc(sizeof(t_segment_row));
	newSegment->id = atoi(list_get(segmentParameters, 0));
	newSegment->segmentSize = atoi(list_get(segmentParameters, 1));
	add_to_memory(newSegment);
}

void execute_delete_segment(int kernelConnection) {
	t_list* segmentParameters = decode_segment_parameters(kernelConnection);
	int segmentId = atoi(list_get(segmentParameters, 0));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("SegmentId to delete: %d", segmentId));
	delete_segment_if_exists(segmentId);
}

void delete_segment_if_exists(int segmentId){
	for(int i = 0; i < list_size(segmentTableGlobal); i++){
		t_segment_row* actualSegment = list_get(segmentTableGlobal, i);
		if(actualSegment->id == segmentId){
			list_remove(segmentTableGlobal, i);
		}
	}
}

void add_to_memory(t_segment_row* segment) {
	t_assignment_algorithm assignmentAlgorithm = get_assignment_algorithm();

	if(assignmentAlgorithm == FIRST){
		add_segment_first_algorithm(segment);
	} else if(assignmentAlgorithm == BEST) {
		add_segment_best_algorithm(segment);
	} else {
		add_segment_worst_algorithm(segment);
	}
}

void add_segment_best_algorithm(t_segment_row* segment) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_segment_best_algorithm] Not implemented yet"));
}

void add_segment_first_algorithm(t_segment_row* segment) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_segment_first_algorithm] Not implemented yet"));
}

void add_segment_worst_algorithm(t_segment_row* segment) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_segment_worst_algorithm] Not implemented yet"));
}
