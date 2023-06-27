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
	//TODO: Hacer create_segment en memory
	send_memory_data_to_memory(pcb);

	//TODO: crear nodo de respuesta para no manejarlo con int
	int response;
	recv(get_memory_connection(), &response, sizeof(int), MSG_WAITALL);

	switch (response){
		case -1:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_create_segment] Ocurrió un error en Memory");
			break;
		case -2:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_create_segment] Memoria solicita compactacion");
			request_compaction_to_memory_and_retry(pcb);
			break;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_create_segment] Ejecutado correctamente");

			t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

			t_segment_row* segmentRow = malloc(sizeof(t_segment_row));
			segmentRow->id = atoi(list_get(instruction->parameters, 0));
			segmentRow->segmentSize = atoi(list_get(instruction->parameters, 1));
			segmentRow->baseDirection = response;
			list_add(pcb->executionContext->segmentTable, segmentRow);


		break;
	}
}

void execute_kernel_delete_segment(t_pcb* pcb){
	//TODO: Hacer delete_segment en memory
	send_memory_data_to_memory(pcb);

	t_list* segmentTable = receive_process_segment_table();

	log_segment_table(segmentTable,get_logger(),LOG_LEVEL_INFO);

	//pcb->executionContext->segmentTable = segmentTable;
}

void send_memory_data_to_memory(t_pcb* pcb){
	t_package* package = create_package();
	t_memory_data* memoryData = malloc(sizeof(t_memory_data));

	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	memoryData->pid = pcb->executionContext->pid;
	memoryData->instruction = instruction;

	log_memory_data(memoryData,get_logger(),LOG_LEVEL_INFO);

	fill_buffer_with_memory_data(memoryData,package);

	send_package(package, get_memory_connection());
}

void request_compaction_to_memory_and_retry(t_pcb* pcb){
	t_package* package = create_package();
	package->operationCode = COMPRESS_SEGMENT_TABLE;

	fill_package_buffer(package, &(pcb->executionContext->pid), sizeof(int));

	//TODO: para probar
	send_package(package, get_memory_connection());
	delete_package(package);

	t_list* segmentTable= receive_process_segment_table();
	pcb->executionContext->segmentTable = segmentTable;

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		"Se realizo la compactacion con exito y se solicita crear el segemento"
	);

	//Volvemos a intentar crear el segmento -> Modular
	execute_kernel_create_segment(pcb);
}




