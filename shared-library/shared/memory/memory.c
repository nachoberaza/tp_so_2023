#include "memory.h"

int get_physical_address(t_log_grouping* logger, t_execution_context* context, char* logicalAddress, int segmentMaxSize) {
	int segment = floor(atoi(logicalAddress) / segmentMaxSize);
	double offset = atoi(logicalAddress) % segmentMaxSize;

	t_instruction* instruction = list_get(context->instructions, context->programCounter);
	char* reg = list_get(instruction->parameters, 1);

	int size = get_amount_of_bytes_per_register(reg, context);

	if(list_size(context->segmentTable) <= segment){
		write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[shared/execution-context - destroy_execution_context] CPU Registers liberados");
	}

	t_segment_row* segmentRow = list_get(context->segmentTable, segment);


	//Seg_fault
	if ((segmentRow->baseDirection + offset + size) > (segmentRow->baseDirection + segmentRow->segmentSize)){
		return -1;
	}

	return segmentRow->baseDirection + offset;
}
