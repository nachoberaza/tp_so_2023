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
	//TODO: Hacer create_segment
	send_current_instruction_to_memory(pcb);

	//TODO: Esta logica varia
	operation_result response;
	recv(get_memory_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_create_segment] Ejecutado correctamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_create_segment] Ocurrió un error en Memory");
}

void execute_kernel_delete_segment(t_pcb* pcb){
	//TODO: Hacer delete_segment
	send_current_instruction_to_memory(pcb);

	//TODO: Esta logica varia
	operation_result response;
	recv(get_memory_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_delete_segment] Ejecutado correctamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_delete_segment] Ocurrió un error en Memory");
}

void send_current_instruction_to_memory(t_pcb* pcb){
	t_package* package = create_package();
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	fill_package_buffer(package, &(instruction->command), sizeof(command));

	int parameterCount = list_size(instruction->parameters);

	fill_package_buffer(package, &parameterCount, sizeof(int));
	for (int j = 0; j < parameterCount; j++){
		char* parameter = list_get(instruction->parameters, j);

		fill_package_buffer(package, parameter, strlen(parameter) + 1);
	}

	send_package(package, get_memory_connection());
}
