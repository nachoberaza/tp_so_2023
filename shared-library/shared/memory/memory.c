#include "memory.h"

int get_physical_address(t_log_grouping* logger, t_execution_context* context, int size, char* logicalAddress, int segmentMaxSize) {
	int segment = floor(atoi(logicalAddress) / segmentMaxSize);
	double offset = atoi(logicalAddress) % segmentMaxSize;

	if(list_size(context->segmentTable) <= segment){
		write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[shared/memory - get_physical_address] No existe el segmento");
	}

	t_segment_row* segmentRow = list_get(context->segmentTable, segment);

	//Seg_fault
	if ((segmentRow->baseDirection + offset + size) > (segmentRow->baseDirection + segmentRow->segmentSize)){
		return -1;
	}

	return segmentRow->baseDirection + offset;
}


void fill_buffer_with_memory_data(t_memory_data* memoryData, t_package* pkg){
	fill_package_buffer(pkg, &(memoryData->pid), sizeof(int));
	fill_buffer_with_instruction(memoryData->instruction,pkg);
}

t_memory_data* decode_memory_data(t_log_grouping* logger, int clientSocket){
	int bufferSize, offset = 0;
	void *buffer;
	t_memory_data* data = malloc(sizeof(t_memory_data));

	buffer = receive_buffer(&bufferSize, clientSocket);

	data->pid = extract_int_from_buffer(buffer, &offset);
	data->instruction = extract_instruction_from_buffer(logger,LOG_LEVEL_INFO,buffer,&offset);

	free(buffer);

	return data;
}

void log_memory_data(t_memory_data* data,t_log_grouping* logger,t_log_level logLevel){
	write_log_grouping(logger,LOG_TARGET_INTERNAL, logLevel, "[shared/memory - log_memory_data]  Memory Data");
	write_log_grouping(logger,LOG_TARGET_INTERNAL, logLevel, string_from_format("Pid: %d", data->pid));
	write_instruction_to_internal_log(logger, logLevel,data->instruction);
}

void log_segment_table(t_list* segmentTable, t_log_grouping* logger, t_log_level logLevel){
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, "[shared/memory - log_segment_table]  Logging segment table:");
	int size = list_size(segmentTable);

	for(int i=0; i<size; i++){
		t_segment_row* row = list_get(segmentTable, i);
		write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, string_from_format("[shared/memory - log_segment_table] Row %d, Pid: %d, id: %d, baseDir: %d, size: %d", i, row->pid, row->id, row->baseDirection, row->segmentSize));
	}
}
