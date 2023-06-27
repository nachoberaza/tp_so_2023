#include "memory-utils.h"

void* memory;
t_list* freeSpacesList;
// TODO: Chequear nombre, den recomendaciones
t_list* segmentTableGlobal;

void create_memory_structures() {
	memory = malloc(get_memory_config()->MEMORY_SIZE);
	freeSpacesList = list_create();
	segmentTableGlobal = list_create();

	t_segment_row* segmentZero = malloc(sizeof(t_segment_row));
	segmentZero->pid=-1;
	segmentZero->id = 0;
	segmentZero->baseDirection = 0;
	segmentZero->segmentSize = get_memory_config()->SEGMENT_ZERO_SIZE;
	list_add(segmentTableGlobal, segmentZero);

	t_segment_row* emptySpace = malloc(sizeof(t_segment_row));
	//id no importa para espacios vacios
	emptySpace->pid=-1;
	emptySpace->id = 0;
	emptySpace->baseDirection = get_memory_config()->SEGMENT_ZERO_SIZE;
	emptySpace->segmentSize = get_memory_config()->MEMORY_SIZE - get_memory_config()->SEGMENT_ZERO_SIZE;
	list_add(freeSpacesList, emptySpace);

}

void* get_memory() {
	return memory;
}

void* get_free_spaces_list() {
	return freeSpacesList;
}

t_list* get_segment_table_global() {
	return segmentTableGlobal;
}

t_list* create_segment_table() {
	t_list* segmentTable = list_create();

	t_segment_row* segmentZero = list_get(segmentTableGlobal,0);
	list_add(segmentTable, segmentZero);

	return segmentTable;
}

operation_result delete_segment_if_exists(int segmentId, int pid){
	for(int i = 0; i < list_size(segmentTableGlobal); i++){
		t_segment_row* actualSegment = list_get(segmentTableGlobal, i);
		if(actualSegment->pid == pid && actualSegment->id == segmentId){
			list_remove(segmentTableGlobal, i);
			return OPERATION_RESULT_OK;
		}
	}
	return OPERATION_RESULT_OK;
}

t_segment_row* get_segment(int segmentId, int pid){
	for(int i = 0; i < list_size(segmentTableGlobal); i++){
		t_segment_row* actualSegment = list_get(segmentTableGlobal, i);
		if(actualSegment->pid == pid && actualSegment->id == segmentId){
			return actualSegment;
		}
	}

	return NULL;
}

int add_to_memory(t_segment_row* segment) {
	t_assignment_algorithm assignmentAlgorithm = get_assignment_algorithm();
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("[utils/memory-utils - add_to_memory] assignment alg: %d", assignmentAlgorithm));
	int res = 0;

	if(assignmentAlgorithm == FIRST){
		res = add_segment_first_algorithm(segment);
	} else if(assignmentAlgorithm == BEST) {
		res = add_segment_best_algorithm(segment);
	} else {
		res = add_segment_worst_algorithm(segment);
	}
	log_segment_table(segmentTableGlobal, get_logger(), LOG_LEVEL_INFO, false);
	log_segment_table(freeSpacesList, get_logger(), LOG_LEVEL_INFO, true);

	return res;
}

int add_segment_best_algorithm(t_segment_row* segment) {
	int size = list_size(freeSpacesList);
	int totalAvailableSize = 0;
	int bestSegmentIndex = -1;
	int bestSegmentSizeDiff = 0;

    for (int i = 0; i < size; i++) {
        t_segment_row* row = list_get(freeSpacesList, i);
        int rowSizeDiff = row->segmentSize - segment->segmentSize;

        if (rowSizeDiff > 0 && (bestSegmentIndex == -1 || rowSizeDiff < bestSegmentSizeDiff)) {
            bestSegmentIndex = i;
            bestSegmentSizeDiff = rowSizeDiff;
        }

        totalAvailableSize += row->segmentSize;
    }

	if(bestSegmentIndex != -1) {
		t_segment_row* bestSegment = list_get(freeSpacesList, bestSegmentIndex);
		add_segment_in_empty_space(bestSegment, segment, bestSegmentIndex);
		return segment->baseDirection;
	}

	if (totalAvailableSize > segment->segmentSize)
		return NEED_COMPACTION;
	return OUT_OF_MEMORY;
}

int add_segment_first_algorithm(t_segment_row* segment) {
	int size = list_size(freeSpacesList);
	int totalAvailableSize = 0;
	for (int i=0; i<size; i++)
	{
		t_segment_row* row = list_get(freeSpacesList, i);
		if (row->segmentSize >= segment->segmentSize){
			add_segment_in_empty_space(row, segment, i);
			return segment->baseDirection;
		}
		totalAvailableSize += row->segmentSize;
	}

	if (totalAvailableSize > segment->segmentSize)
		return NEED_COMPACTION;
	return OUT_OF_MEMORY;
}

int add_segment_worst_algorithm(t_segment_row* segment) {
	int size = list_size(freeSpacesList);
	int totalAvailableSize = 0;
	int worstSegmentIndex = -1;
	int worstSegmentSizeDiff = 0;

    for (int i = 0; i < size; i++) {
        t_segment_row* row = list_get(freeSpacesList, i);
        int rowSizeDiff = row->segmentSize - segment->segmentSize;

        if (rowSizeDiff > worstSegmentSizeDiff) {
            worstSegmentIndex = i;
            worstSegmentSizeDiff = rowSizeDiff;
        }

        totalAvailableSize += row->segmentSize;
    }

	if(worstSegmentIndex != -1) {
		t_segment_row* worstSegment = list_get(freeSpacesList, worstSegmentIndex);
		add_segment_in_empty_space(worstSegment, segment, worstSegmentIndex);
		return segment->baseDirection;
	}

	if (totalAvailableSize > segment->segmentSize)
		return NEED_COMPACTION;
	return OUT_OF_MEMORY;
}


void add_segment_in_empty_space(t_segment_row* row, t_segment_row* segment, int i){
	segment->baseDirection = row->baseDirection;
	list_add(segmentTableGlobal, segment);
	if (row->segmentSize == segment->segmentSize)
	{
		list_remove(freeSpacesList, i);
		return;
	}
	row->baseDirection += segment->segmentSize;
	row->segmentSize -= segment->segmentSize;
}
