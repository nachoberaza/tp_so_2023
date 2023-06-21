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

	buffer = receive_buffer(&bufferSize, get_memory_connection());

	t_list* segmentTable  = extract_segment_table_from_buffer(buffer, &offset);

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

void execute_kernel_create_segment(t_pcb* pcb){
	char* segmentId = list_get(pcb->executionContext->reason->parameters, 0);
	char* segmentSize = list_get(pcb->executionContext->reason->parameters, 1);
	int parametersCount = 2;
	t_package* package = create_package();
	package->operationCode = CREATE_SEGMENT_KERNEL;
	fill_package_buffer(package, &parametersCount, sizeof(int));
	fill_package_buffer(package, segmentId, strlen(segmentId) + 1);
	fill_package_buffer(package, segmentSize, strlen(segmentSize) + 1);
	send_package(package, get_memory_connection());
}

void execute_kernel_delete_segment(t_pcb* pcb){
	char* segmentId = list_get(pcb->executionContext->reason->parameters, 0);
	int parametersCount = 1;
	t_package* package = create_package();
	package->operationCode = DELETE_SEGMENT_KERNEL;
	fill_package_buffer(package, &parametersCount, sizeof(int));
	fill_package_buffer(package, segmentId, strlen(segmentId) + 1);
	send_package(package, get_memory_connection());
}
