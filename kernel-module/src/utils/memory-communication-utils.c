#include "memory-communication-utils.h"

t_list* init_process_segment_table(int pid){
	t_package* package = create_package();
	package->operationCode = CREATE_SEGMENT_TABLE;

	fill_package_buffer(package, &(pid), sizeof(int));

	send_package(package, get_memory_connection());
	delete_package(package);

	return receive_process_segment_table();
}

t_list* receive_process_segment_table() {
	int bufferSize, offset = 0;
	void *buffer;

	receive_operation_code(get_memory_connection());

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

	send_memory_data_to_memory(pcb);

	int response;
	recv(get_memory_connection(), &response, sizeof(int), MSG_WAITALL);

	switch (response){
		case OUT_OF_MEMORY:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/memory-communication-utils - execute_kernel_create_segment] Ocurrió un error en Memory");
			
			add_error_in_execution_context_reason(pcb->executionContext->reason, REASON_ERROR,ERROR_OUT_OF_MEMORY);
			move_to_exit(pcb);
			break;
		case NEED_COMPACTION:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/memory-communication-utils - execute_kernel_create_segment] Memoria solicita compactacion");
			request_compaction_to_memory_and_retry(pcb);
			break;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/memory-communication-utils - execute_kernel_create_segment] Ejecutado correctamente");

			t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

			t_segment_row* segmentRow = malloc(sizeof(t_segment_row));
			segmentRow->pid=pcb->executionContext->pid;
			segmentRow->id = atoi(list_get(instruction->parameters, 0));
			segmentRow->segmentSize = atoi(list_get(instruction->parameters, 1));
			segmentRow->baseDirection = response;
			list_add(pcb->executionContext->segmentTable, segmentRow);


		break;
	}
}

void execute_kernel_delete_segment(t_pcb* pcb){
	send_memory_data_to_memory(pcb);

	t_list* segmentTable = receive_process_segment_table();

	update_segment_table_in_all_proccesses(segmentTable);
}

void update_segment_table_in_all_proccesses(t_list* newSegmentTable){
	int size = list_size(newSegmentTable);
	int sizePcbList = list_size(get_short_term_list());
	t_segment_row* segmentZero = list_get(newSegmentTable,0);



	for(int i=0; i< sizePcbList; i++){
		t_pcb* pcb = list_get(get_short_term_list(),i);

		list_clean(pcb->executionContext->segmentTable);
		list_add(pcb->executionContext->segmentTable, segmentZero);
	}

	for(int i=0; i< size; i++){
		t_segment_row* segment = list_get(newSegmentTable,i);
		t_pcb* pcb = search_pcb_by_pid(segment->pid);
		if(pcb != NULL){
			list_add(pcb->executionContext->segmentTable,segment);
		}
	}

	for(int i=0; i< sizePcbList; i++){
		t_pcb* pcb = list_get(get_short_term_list(),i);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
				string_from_format("[utils/memory-communication-utils - update_segment_table_in_all_proccesses] Segment Table Pid : %d",pcb->executionContext->pid));
		log_segment_table(pcb->executionContext->segmentTable,get_logger(),LOG_LEVEL_TRACE, false);
	}
}



t_pcb* search_pcb_by_pid(int pid){
	int size = list_size(get_short_term_list());

	for(int i=0; i< size; i++){
		t_pcb* pcb = list_get(get_short_term_list(),i);
		if(pcb->executionContext->pid == pid){
			return pcb;
		}
	}

	return NULL;
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
	int printedLog = 0;
	t_package* package = create_package();
	package->operationCode = COMPRESS_SEGMENT_TABLE;

	fill_package_buffer(package, &(pcb->executionContext->pid), sizeof(int));


	while(get_count_fs_operations() > 0){
		if(!printedLog){
			write_to_log(
						LOG_TARGET_INTERNAL,
						LOG_LEVEL_TRACE,
						"Compactación: Esperando Fin de Operaciones de FS"
				);
			printedLog =1;
		}
	}


	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			"Compactación: Se solicitó compactación"
	);

	send_package(package, get_memory_connection());
	delete_package(package);

	t_list* segmentTable= receive_process_segment_table();
	update_segment_table_in_all_proccesses(segmentTable);

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		"Se finalizó el proceso de compactación"
	);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		"Se realizo la compactacion con exito y se solicita crear el segemento"
	);

	//Volvemos a intentar crear el segmento -> Modular
	execute_kernel_create_segment(pcb);
}




