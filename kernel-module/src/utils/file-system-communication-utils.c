#include "file-system-communication-utils.h"

t_list *openFilesTable;
int countFileOperations;

void start_open_files_table_list() {
	openFilesTable = list_create();
	countFileOperations = 0;
}

t_list* get_open_files_table_list(){
	return openFilesTable;
}

t_open_file_row* new_open_file_row(char *fileName){
	t_open_file_row *openFileRow = malloc(sizeof(t_open_file_row));
	openFileRow->file = fileName;
	openFileRow->pointer = 0;
	return openFileRow;
}

void execute_kernel_f_open(t_pcb* pcb){
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	char *fileName = list_get(instruction->parameters, 0);

	if(!open_files_table_contains(fileName)){

		send_instruction_to_fs(instruction, pcb->executionContext->pid);

		int response;
		recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

		if(response == OPERATION_RESULT_ERROR){
			instruction->command = F_CREATE;
			send_instruction_to_fs(instruction, pcb->executionContext->pid);

			recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
					string_from_format("[utils/file-system-communication-utils - execute_kernel_f_open] Respuesta del FS al F_CREATE : %d ", response));

		}

		add_file_to_open_files_table(pcb,fileName);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/file-system-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
	}

	t_resource* resource = get_resource(openFilesTable, fileName);
	request_file(resource,pcb);
	add_file_to_process_open_files_table(pcb,instruction);

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("[utils/file-system-communication-utils -execute_kernel_f_open] PID: %d - Abrir Archivo: %s",
				pcb->executionContext->pid,resource->name)
	);
}

void execute_kernel_f_close(t_pcb* pcb){
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	char *fileName = list_get(instruction->parameters, 0);
	t_resource* resource = get_resource(openFilesTable, fileName);
	remove_file_from_process_open_files_table(pcb,instruction);
	if(list_is_empty(resource->blocked)){
		remove_file_from_open_files_table(resource);
	}else{
		t_pcb* processBloqued = list_get(resource->blocked, 0);
		list_remove(resource->blocked, 0);
		move_to_ready(processBloqued);
	}

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("[utils/file-system-communication-utils - execute_kernel_f_close] PID: %d - Cerrar Archivo: %s",
				pcb->executionContext->pid,resource->name)
	);
}
void remove_file_from_open_files_table(t_resource* resource){
	list_remove(openFilesTable, get_resource_index(openFilesTable,resource->name));
}

void execute_kernel_f_seek(t_pcb* pcb){
	//TODO: Hacer fopen
	t_package* package = create_package();
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	send_instruction_to_fs(instruction, pcb->executionContext->pid);

	//TODO: Esta logica varia
	operation_result response;
	recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_open] Ocurrió un error en FS");
}

void execute_kernel_f_read(t_pcb* pcb){
	//TODO: Hacer fopen
	t_package* package = create_package();
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	fill_package_buffer(package, &(instruction->command), sizeof(command));
	int parameterCount = list_size(instruction->parameters);

	fill_package_buffer(package, &parameterCount, sizeof(int));
	for (int j = 0; j < parameterCount; j++){
		char* parameter = list_get(instruction->parameters, j);

		fill_package_buffer(package, parameter, strlen(parameter) + 1);
	}

	send_package(package, get_file_system_connection());

	//TODO: Esta logica varia
	operation_result response;
	recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_open] Ocurrió un error en FS");
}

void execute_kernel_f_write(t_pcb* pcb){
	t_package* package = create_package();
	t_instruction* currentInstruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	t_instruction* instruction = duplicate_instruction(currentInstruction);

	list_add(instruction->parameters, list_get(pcb->executionContext->reason->parameters, 0));
	list_add(instruction->parameters, "0");

	write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, instruction);

	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = pcb->executionContext->pid;
	memoryData->instruction = instruction;

	fill_buffer_with_memory_data(memoryData,package);

	send_package(package, get_file_system_connection());

	countFileOperations++;
	move_to_blocked(pcb);

	pthread_t ioThread;
	int threadStatus = pthread_create(&ioThread, NULL, (void *) await_f_write, (void*)pcb);
	if (threadStatus != 0){
	   write_to_log(
	    	LOG_TARGET_INTERNAL,
			LOG_LEVEL_INFO,
			string_from_format("[utils/cpu-communication-utils - execute_kernel_f_write] Hubo un problema al crear el ioThread - Reason: %s", strerror(threadStatus))
	   );
	   exit(EXIT_FAILURE);
	}
}

t_instruction* duplicate_instruction(t_instruction* currentInstruction){
	t_instruction* instruction = malloc(sizeof(t_instruction));
	instruction->command = currentInstruction->command;
	instruction->parameters = list_create();
	int size = list_size(currentInstruction->parameters);

	for (int i = 0; i < size; i++){
		list_add(instruction->parameters, string_duplicate(list_get(currentInstruction->parameters, i)));
	}

	return instruction;
}

void await_f_write(t_pcb* pcb){
	operation_result response;
	recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_write] Ejecutado correctamente");
		countFileOperations--;
		move_pcb_to_short_term_end(pcb);
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_write] Ocurrió un error en FS");
}

void execute_kernel_f_truncate(t_pcb* pcb){
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);
	send_instruction_to_fs(instruction, pcb->executionContext->pid);

	//TODO: Esta logica varia
	operation_result response;
	recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_open] Ocurrió un error en FS");
}

void add_file_to_process_open_files_table(t_pcb* pcb, t_instruction* instruction){
	t_open_file_row *openFile = new_open_file_row(list_get(instruction->parameters, 0));
	add_file(pcb,openFile);
}

void remove_file_from_process_open_files_table(t_pcb* pcb, t_instruction* instruction){
	char* fileName = list_get(instruction->parameters, 0);
	int fileIndex = get_open_file_index(pcb->openFilesTable,fileName);
	if(fileIndex == NULL)
			exit(EXIT_FAILURE);

	list_remove(pcb->openFilesTable, fileIndex);
}

int get_open_file_index(t_list * list,char* fileName){
	int size = list_size(list);

	for(int i=0; i< size;i++){
		t_open_file_row* openFileRow = list_get(list,i);

		if(!strcmp(openFileRow->file,fileName)){
			return i;
		}
	}

	return NULL;
}


int open_files_table_contains(char *fileName){
	t_resource* resource = get_resource(openFilesTable, fileName);
	return resource != NULL;
}

void add_file_to_open_files_table(t_pcb* pcb,char *fileName){
	t_resource* resource = create_resource(fileName, "1");
	list_add(openFilesTable,resource);
}

void request_file(t_resource* resource, t_pcb* pcb){
	if(resource->instances <= 0){
		write_to_log(
					LOG_TARGET_MAIN,
					LOG_LEVEL_INFO,
					string_from_format("[utils/file-system-communication-utils - block_process_f_open] PID: %d - Solicito por: %s",
							pcb->executionContext->pid,resource->name)
				);
				move_to_blocked(pcb);
				list_add(resource->blocked, pcb);
	}
	resource->instances--;
}

void send_instruction_to_fs(t_instruction* instruction, int pid){
	t_package* package = create_package();
	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = pid;
	memoryData->instruction = instruction;

	fill_buffer_with_memory_data(memoryData, package);

	send_package(package, get_file_system_connection());
}

void send_current_instruction_to_fs(t_pcb* pcb){
	t_package* package = create_package();
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);
	fill_buffer_with_instruction(instruction,package);
	send_package(package, get_file_system_connection());
}
