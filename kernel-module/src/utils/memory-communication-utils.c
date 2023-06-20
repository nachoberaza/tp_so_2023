#include "memory-communication-utils.h"

t_list* init_process_segment_table(int pid){
	t_package* package = create_package();
	package->operationCode = CREATE_SEGMENT_TABLE;

	fill_package_buffer(package, &(pid), sizeof(int));

	send_package(package, get_memory_connection());
	delete_package(package);

	receive_operation_code(get_memory_connection());
	return receive_process_segment_table();
}

t_list* receive_process_segment_table() {
	int bufferSize, offset = 0;
	void *buffer;
	t_list* segmentTable = list_create();

	buffer = receive_buffer(&bufferSize, get_memory_connection());

	int segmentTableCount = extract_int_from_buffer(buffer, &offset);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		string_from_format("Recibí tabla con %d elementos", segmentTableCount)
	);

	for (int i = 0; i < segmentTableCount; i++){
		t_segment_row* segmentRow = malloc(sizeof(t_segment_row));

		segmentRow->id = extract_int_from_buffer(buffer, &offset);
		segmentRow->baseDirection = extract_int_from_buffer(buffer, &offset);
		segmentRow->segmentSize = extract_int_from_buffer(buffer, &offset);
		list_add(segmentTable, segmentRow);
	}

	free(buffer);

	return segmentTable;
}

void delete_process_segment_table(int pid){
	t_package* package = create_package();
	package->operationCode = DELETE_SEGMENT_TABLE;

	fill_package_buffer(package, &(pid), sizeof(int));

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		string_from_format("Pid de la tabla a liberar :  %d", pid)
	);

	//TODO: para probar
	send_package(package, get_memory_connection());
	delete_package(package);

	receive_operation_code(get_memory_connection());
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			"Elimine/Libere tabla de segmentos"
	);

}
