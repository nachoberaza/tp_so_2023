#include "file-system-communication-utils.h"

t_list *openFilesTable;
int countFSOperations;

void start_open_files_table_list() {
	openFilesTable = list_create();
	countFSOperations = 0;
}

t_list* get_open_files_table_list(){
	return openFilesTable;
}

int get_count_fs_operations(){
	return countFSOperations;
}

void execute_kernel_f_open(t_pcb* pcb){
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	char * fileNameTemp = list_get(instruction->parameters, 0);
	char * fileName = string_duplicate(fileNameTemp);
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

void execute_kernel_f_seek(t_pcb* pcb){
	t_instruction* instruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	char* fileName = list_get(instruction->parameters, 0);
	char* pointer = list_get(instruction->parameters, 1);

	t_open_file_row* openFile = get_open_file(pcb->openFilesTable, fileName);

	openFile->pointer = atoi(pointer);

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d - Actualizar puntero Archivo: %s - Puntero %d",pcb->executionContext->pid,fileName,openFile->pointer));

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
			string_from_format("[utils/cpu-communication-utils - execute_kernel_f_seek] Archivo %s apuntando a %d", openFile->file, openFile->pointer));
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

	char* fileName = list_get(currentInstruction->parameters, 0);
	t_open_file_row* openFile = get_open_file(pcb->openFilesTable, fileName);

	list_add(instruction->parameters, list_get(pcb->executionContext->reason->parameters, 0));
	char* pointer = string_itoa(openFile->pointer);
	list_add(instruction->parameters, pointer);

	write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, instruction);

	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = pcb->executionContext->pid;
	memoryData->instruction = instruction;

	fill_buffer_with_memory_data(memoryData,package);

	send_package(package, get_file_system_connection());

	//REVISAR LA INFO DEL LOG
	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("PID: %d -  Escribir Archivo: %s - Puntero %s - Dirección Memoria %d - Tamaño %d",
				pcb->executionContext->pid,fileName,pointer,list_get(currentInstruction->parameters, 1),list_get(currentInstruction->parameters, 2)));

	countFSOperations++;
	move_to_blocked(pcb);

	pthread_t ioThread;
	int threadStatus = pthread_create(&ioThread, NULL, (void *) await_f_write, (void*)pcb);
	if (threadStatus != 0){
	   write_to_log(
	    	LOG_TARGET_INTERNAL,
			LOG_LEVEL_ERROR,
			string_from_format("[utils/cpu-communication-utils - execute_kernel_f_write] Hubo un problema al crear el ioThread - Reason: %s", strerror(threadStatus))
	   );
	   exit(EXIT_FAILURE);
	}
}

void await_f_write(t_pcb* pcb){
	operation_result response;
	recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_write] Ejecutado correctamente");
		countFSOperations--;
		move_pcb_to_short_term_end(pcb);
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/cpu-communication-utils - execute_kernel_f_write] Ocurrió un error en FS");
}

void execute_kernel_f_truncate(t_pcb* pcb){
	t_package* package = create_package();
	t_instruction* currentInstruction = list_get(pcb->executionContext->instructions, pcb->executionContext->programCounter - 1);

	t_instruction* instruction = duplicate_instruction(currentInstruction);
	char* fileName = list_get(currentInstruction->parameters,0);
	char* size = list_get(currentInstruction->parameters,2);

	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = pcb->executionContext->pid;
	memoryData->instruction = instruction;

	fill_buffer_with_memory_data(memoryData,package);

	send_package(package, get_file_system_connection());

	operation_result response;
	recv(get_file_system_connection(), &response, sizeof(int), MSG_WAITALL);

	if(response == OPERATION_RESULT_OK){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/fs-communication-utils - execute_kernel_f_open] Ejecutado correctamente");
		write_to_log(
			LOG_TARGET_MAIN,
			LOG_LEVEL_INFO,
			string_from_format("PID: %d - Archivo: %s - Tamaño: %s",
						pcb->executionContext->pid,fileName,size));
		return;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/fs-communication-utils - execute_kernel_f_open] Ocurrió un error en FS");
}

void add_file_to_process_open_files_table(t_pcb* pcb, char* fileName){
	t_open_file_row *openFile = create_open_file_row(fileName);
	add_file(pcb,openFile);
}

int open_files_table_contains(char *fileName){
	t_resource* resource = get_resource(openFilesTable, fileName);
	return resource != NULL;
}

void add_file_to_open_files_table(t_pcb* pcb,char *fileName){
	t_resource* resource = create_resource(fileName, "1");
	list_add(openFilesTable,resource);
}

int get_open_file_index(t_list * list,char* fileName){
	int size = list_size(list);

	for(int i=0; i< size;i++){
		t_open_file_row* openFileRow = list_get(list, i);
		//write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
			//			string_from_format("[utils/fs-communication-utils - get_open_file_index] Nombre archivo: %s , fileName: %s",openFileRow->file,fileName));
		if(!strcmp(openFileRow->file,fileName)){
			return i;
		}
	}

	return -1;
}

t_open_file_row* get_open_file(t_list * list, char* fileName){
	int size = list_size(list);

	for(int i=0; i< size;i++){
		t_open_file_row* openFileRow = list_get(list,i);
		if(!strcmp(openFileRow->file,fileName)){
			return openFileRow;
		}
	}

	return -1;
}

void process_release_all_files(t_pcb* pcb){
	int size = list_size(pcb->openFilesTable);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
		string_from_format("[utils/fs-communication-utils - process_release_all_files] Archivos abiertos pid :%d , files :%d",pcb->executionContext->pid,size));

	for(int i=size -1; i>=0; i--){
		t_open_file_row *openFile = list_get(pcb->openFilesTable,i);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
				string_from_format("[utils/fs-communication-utils - process_release_all_files] Nombre archivo: %s", openFile->file));
		process_file_release(pcb, openFile->file);
	}

	list_destroy_and_destroy_elements(pcb->openFilesTable, (void*) destroy_open_files_row);
}

void process_file_release(t_pcb* pcb, char* fileName){
	t_resource* resource = get_resource(openFilesTable, fileName);
	//Elimino la lista y todos los archivos abiertos del
	//remove_file_from_process_open_files_table(pcb,fileName);

	if(list_is_empty(resource->blocked)){
		remove_file_from_open_files_table(resource);
	}else{
		t_pcb* processBloqued = list_get(resource->blocked, 0);
		list_remove(resource->blocked, 0);
		move_to_ready(processBloqued);
	}
}

void remove_file_from_open_files_table(t_resource* resource){
	int resourceIndex = get_resource_index(openFilesTable,resource->name);
	if(resourceIndex == -1){
			return;
	}

	list_remove(openFilesTable, resourceIndex);
}

void remove_file_from_process_open_files_table(t_pcb* pcb, char* fileName){
	int fileIndex = get_open_file_index(pcb->openFilesTable, fileName);

	if(fileIndex == -1){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,
				string_from_format("[utils/fs-communication-utils - remove_file_from_process_open_files_table] No se encontro el index de %s",fileName));
		return;
	}
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG,
						string_from_format("[utils/fs-communication-utils - remove_file_from_process_open_files_table] File: %s,Index : %d",fileName,fileIndex));

	list_remove(pcb->openFilesTable, fileIndex);
}

void send_instruction_to_fs(t_instruction* instruction, int pid){
	t_package* package = create_package();
	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = pid;
	memoryData->instruction = instruction;

	fill_buffer_with_memory_data(memoryData, package);

	send_package(package, get_file_system_connection());
}


