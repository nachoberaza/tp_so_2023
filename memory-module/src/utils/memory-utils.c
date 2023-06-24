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

operation_result delete_segment_if_exists(int segmentId){
	for(int i = 0; i < list_size(segmentTableGlobal); i++){
		t_segment_row* actualSegment = list_get(segmentTableGlobal, i);
		if(actualSegment->id == segmentId){
			list_remove(segmentTableGlobal, i);
			return OPERATION_RESULT_OK;
		}
	}
	//TODO: Cambiar a error cuando se use segmentTableGlobal
	return OPERATION_RESULT_OK;
}

operation_result add_to_memory(t_segment_row* segment) {
	t_assignment_algorithm assignmentAlgorithm = get_assignment_algorithm();

	if(assignmentAlgorithm == FIRST){
		return add_segment_first_algorithm(segment);
	} else if(assignmentAlgorithm == BEST) {
		return add_segment_best_algorithm(segment);
	} else {
		return add_segment_worst_algorithm(segment);
	}
}

operation_result add_segment_best_algorithm(t_segment_row* segment) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_segment_best_algorithm] Not implemented yet"));
	return OPERATION_RESULT_OK;
}

operation_result add_segment_first_algorithm(t_segment_row* segment) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_segment_first_algorithm] Not implemented yet"));
	return OPERATION_RESULT_OK;
}

operation_result add_segment_worst_algorithm(t_segment_row* segment) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_segment_worst_algorithm] Not implemented yet"));
	return OPERATION_RESULT_OK;
}
