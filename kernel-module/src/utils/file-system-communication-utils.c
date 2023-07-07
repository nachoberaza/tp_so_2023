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
	string_trim(&fileName);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_INFO,
	string_from_format("Abrir Archivo: %s",fileName));

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
		add_file_to_process_open_files_table(pcb,fileName);

		write_to_log(
				LOG_TARGET_MAIN,
				LOG_LEVEL_INFO,
				string_from_format("PID: %d - Abrir Archivo: %s",pcb->executionContext->pid,fileName));

		return;
	}


	t_resource* resource = get_resource(openFilesTable, fileName);
	add_file_to_process_open_files_table(pcb,fileName);

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d - Bloqueado por: %s", pcb->executionContext->pid,fileName)
	);

	list_add(resource->blocked, pcb);
	move_to_blocked(pcb);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/file-system-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
}

void execute_kernel_f_close(t_pcb* pcb){
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	char *fileName = list_get(instruction->parameters, 0);

	process_file_release(pcb,fileName);

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d - Cerrar Archivo: %s",pcb->executionContext->pid, fileName));
}

void remove_file_from_open_files_table(t_resource* resource){
	int i = get_resource_index(openFilesTable,resource->name);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
						string_from_format("[utils/fs-communication-utils - process_file_release]recurso: %s,Index : %d",resource->name,i));
	list_remove(openFilesTable, i);
}

void process_file_release(t_pcb* pcb, char* fileName){
	t_resource* resource = get_resource(openFilesTable, fileName);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
					string_from_format("[utils/fs-communication-utils - process_file_release] Nombre recurso: %s",resource->name));
	remove_file_from_process_open_files_table(pcb,fileName);

	if(list_is_empty(resource->blocked)){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/fs-communication-utils - process_file_release] Blocked vacia");
		remove_file_from_open_files_table(resource);
	}else{
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/fs-communication-utils - process_file_release] Blocked No vacia");
		t_pcb* processBloqued = list_get(resource->blocked, 0);
		list_remove(resource->blocked, 0);
		move_to_ready(processBloqued);
	}
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
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/fs-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/fs-communication-utils - execute_kernel_f_open] Ocurrió un error en FS");
}

void add_file_to_process_open_files_table(t_pcb* pcb, char* fileName){
	t_open_file_row *openFile = new_open_file_row(fileName);
	add_file(pcb,openFile);
}

void remove_file_from_process_open_files_table(t_pcb* pcb, char* fileName){
	int fileIndex = get_open_file_index(pcb->openFilesTable, fileName);

	if(fileIndex == NULL){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,
				"[utils/fs-communication-utils - remove_file_from_process_open_files_table] No encontre indice");
	}

	list_remove(pcb->openFilesTable, fileIndex);
}

int get_open_file_index(t_list * list,char* fileName){
	int size = list_size(list);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
		string_from_format("[utils/fs-communication-utils - get_open_file_index] Size list file: %d",size));

	for(int i=0; i< size;i++){
		t_open_file_row* openFileRow = list_get(list,i);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
						string_from_format("[utils/fs-communication-utils - get_open_file_index] Nombre archivo: %s , fileName: %s",openFileRow->file,fileName));
		if(!strcmp(openFileRow->file,fileName)){
			return i;
		}
	}

	return -1;
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
					string_from_format("PID: %d - Solicito por: %s",
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

void process_release_all_files(t_pcb* pcb){
	int size = list_size(pcb->openFilesTable);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
		string_from_format("[utils/fs-communication-utils - process_release_all_files] Archivos abiertos pid :%d , files :%d",pcb->executionContext->pid,size));

	for(int i=size -1; i>=0; i--){
		t_open_file_row *openFile = list_get(pcb->openFilesTable,i);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
				string_from_format("[utils/fs-communication-utils - process_release_all_files] Nombre archivo: %s",openFile->file));
		process_file_release(pcb, openFile->file);
	}
}
