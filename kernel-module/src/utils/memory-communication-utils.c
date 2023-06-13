#include "memory-communication-utils.h"

t_list* init_process_segment_table(){
	t_package* package = create_package();
	package->operationCode = CREATE_SEGMENT_TABLE;
	send_package(package, get_memory_connection());
	receive_operation_code(get_memory_connection());
	t_list* segmentTable = receive_process_segment_table();
	return segmentTable;
}

t_list* receive_process_segment_table() {
	int bufferSize, offset = 0;
	void *buffer;
	t_list* segmentTable = list_create();
	t_segment_row* segmentZero = malloc(sizeof(t_segment_row));

	buffer = receive_buffer(&bufferSize, get_memory_connection());
	segmentZero->id = extract_int_from_buffer(buffer, &offset);
	segmentZero->baseDirection = extract_int_from_buffer(buffer, &offset);
	segmentZero->segmentSize = extract_int_from_buffer(buffer, &offset);
	list_add(segmentTable, segmentZero);
	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("Recibi la tabla")
	);
	return segmentTable;
}
