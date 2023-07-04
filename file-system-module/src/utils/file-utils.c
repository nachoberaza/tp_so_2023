#include "file-utils.h"


operation_result execute_fs_f_create(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_create] Ejecutando F_CREATE");

	t_fcb* fcb = create_fcb_from_instruction(instruction);
	FILE *fcbPointer  = fopen(get_fcb_path(instruction), "w");
	persist_fcb(fcbPointer,fcb);
	log_fcb(fcb);
	fclose(fcbPointer);

	list_add(get_fcb_list(), fcb);

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("Crear Archivo: %s",
				list_get(instruction->parameters, 0))
	);
	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_open(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_open] Ejecutando F_OPEN");

	char* fileName = list_get(instruction->parameters, 0);

	t_fcb* fcb = get_fcb_by_name(fileName);

	if (fcb == NULL)
		return OPERATION_RESULT_ERROR;

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("Abrir Archivo: %s",
				fileName)
	);

	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_read(t_memory_data* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_read] Ejecutando F_READ");
	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_write(t_memory_data* memoryData){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_write] Ejecutando F_WRITE");

	int size = atoi(list_get(memoryData->instruction->parameters, 2));
	int address = atoi(list_get(memoryData->instruction->parameters, 3));

	send_f_write_to_memory(memoryData, size, address);

	int bufferSize, offset = 0;
	void *buffer;

	receive_operation_code(get_memory_connection());
	buffer = receive_buffer(&bufferSize, get_memory_connection());
	//No tiene /0
	char* value = extract_string_from_buffer(buffer, &offset);
	string_append(&value, "\0");

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_INFO,
		string_from_format("Value recibido de memoria: %s",
				value)
	);

	int blockPointer = atoi(list_get(memoryData->instruction->parameters, 4));


	write_in_block(memoryData->instruction, value);
	return OPERATION_RESULT_OK;
}


void send_f_write_to_memory(t_memory_data* context, int size, int physicalAddress){
	t_package* package = create_package();
	t_instruction* memoryInstruction = malloc(sizeof(t_instruction));

	memoryInstruction->command = F_WRITE;
	memoryInstruction->parameters = list_create();
	list_add(memoryInstruction->parameters, string_itoa(physicalAddress));
	list_add(memoryInstruction->parameters, string_itoa(size));

	write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, memoryInstruction);

	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = context->pid;
	memoryData->instruction = memoryInstruction;


	fill_buffer_with_memory_data(memoryData, package);

	send_package(package, get_memory_connection());
}

operation_result execute_fs_f_truncate(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_truncate] Ejecutando F_TRUNCATE");
	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_seek(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_seek] Ejecutando F_SEEK");
	return OPERATION_RESULT_OK;
}


