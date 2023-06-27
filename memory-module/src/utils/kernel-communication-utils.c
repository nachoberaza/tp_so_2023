#include "kernel-communication-utils.h"

void listen_kernel_connection(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Escuchando al modulo kernel");
	while(1){
		operation_code operationCode = receive_operation_code(clientSocketId);
		write_log_grouping(get_logger(), LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("kernel thread - Operation code : %d", operationCode));
		switch(operationCode){
			case CREATE_SEGMENT_TABLE:
				execute_memory_create_segment_table(clientSocketId);
				break;
			case DELETE_SEGMENT_TABLE:
				execute_memory_delete_segment_table(clientSocketId);
				break;
			case COMPRESS_SEGMENT_TABLE:
				execute_memory_compress_segment_table(clientSocketId);
				break;
			case PACKAGE:
				execute_memory_kernel_instruction(clientSocketId);
				break;
			default:
				write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando DEFAULT");
				break;
		}
	}
}

void execute_memory_kernel_instruction(int clientSocketId){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando PACKAGE");

	t_memory_data* data = decode_memory_data(get_logger(), clientSocketId);

	log_memory_data(data,get_logger(), LOG_LEVEL_INFO);

	switch (data->instruction->command){
		case CREATE_SEGMENT:
			execute_memory_create_segment(data,clientSocketId);
			break;
		case DELETE_SEGMENT:
			execute_memory_delete_segment(data,clientSocketId);
			break;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection] Comando no pertenece a Memory");
			return;
		break;
	}
}

void execute_memory_compress_segment_table(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando COMPRESS_SEGMENT_TABLE");
	//TODO
}

void execute_memory_delete_segment_table(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando DELETE_SEGMENT_TABLE");
	buffer = receive_buffer(&bufferSize, clientSocketId);
	int pid = extract_int_from_buffer(buffer, &offset);

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO, string_from_format("Eliminación de Proceso PID: %d",pid));

	operation_result result = OPERATION_RESULT_OK;

	send(clientSocketId, &result, sizeof(operation_result), NULL);

	//TODO
}

void execute_memory_create_segment_table(int clientSocketId){
	int bufferSize, offset = 0;
	void *buffer;

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_kernel_connection - kernel thread] Ejecutando CREATE_SEGMENT_TABLE");

	buffer = receive_buffer(&bufferSize, clientSocketId);
	int pid = extract_int_from_buffer(buffer, &offset);

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO, string_from_format("Creación de Proceso PID: %d",pid));

	t_list* segmentTable = create_segment_table();
	t_package* package = create_package();
	fill_package_with_segment_table(package, segmentTable);
	send_package(package, clientSocketId);
}

void execute_memory_create_segment(t_memory_data* data, int clientSocketId){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_memory_create_segment] Ejecutando create segment");

	t_segment_row* newSegment = malloc(sizeof(t_segment_row));
	newSegment->id = atoi(list_get(data->instruction->parameters, 0));
	newSegment->segmentSize = atoi(list_get(data->instruction->parameters, 1));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Segmento a crear - id: %d - size: %d", newSegment->id, newSegment->segmentSize));


	//Por ahora lo manejmos como como int
	//Success: direccion base
	//Error: out of memory -1
	//Compactar : -2
	int baseDirection = add_to_memory(newSegment);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("BaseDirection del nuevo segmento : %d", baseDirection));

	if(baseDirection != -1 && baseDirection != -2){
		write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO,
					string_from_format("PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d",data->pid,newSegment->id,baseDirection,newSegment->segmentSize));
	}

	send(clientSocketId, &baseDirection, sizeof(int), NULL);
}

void execute_memory_delete_segment(t_memory_data* data, int clientSocketId){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_memory_delete_segment] Ejecutando delete segment");

	int segmentId = atoi(list_get(data->instruction->parameters, 0));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("SegmentId to delete: %d", segmentId));
	t_segment_row* segment = get_segment(segmentId);
	if(segment == NULL){
		write_to_log(LOG_TARGET_ALL, LOG_LEVEL_ERROR, string_from_format("No existe el segmento: %d", segmentId));
	}

	//validar
	operation_result result = delete_segment_if_exists(segmentId);

	segment->id = -1;
	//Esto deberia estar dentro del delete pero tu vieja va a refactorizar
	list_add_sorted(get_free_spaces_list(), segment, (void*)compare_base_segment_row);

	log_segment_table(get_segment_table_global(),get_logger(),LOG_LEVEL_INFO);

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO,
					string_from_format("PID: %d - Eliminar Segmento: %d - Base: %d - TAMAÑO: %d",data->pid,segmentId,segment->baseDirection,segment->segmentSize));

	t_package* package = create_package();
	fill_package_with_segment_table(package, get_segment_table_global());
	send_package(package, clientSocketId);
}

int compare_base_segment_row(void* a, void* b){
	t_segment_row* rowA = (t_segment_row*)a;
	t_segment_row* rowB = (t_segment_row*)b;

	if(rowA->baseDirection <= rowB->baseDirection){
		return 1;
	}

	return 0;
}

