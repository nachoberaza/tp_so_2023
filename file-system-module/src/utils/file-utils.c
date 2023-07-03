#include "file-utils.h"


operation_result execute_fs_f_create(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_create] Ejecutando F_CREATE");

	char* filePath = get_file_path(instruction);

	FILE *filePointer  = fopen(filePath, "w");
	fclose(filePointer);

	t_fcb* fcb = create_fcb_from_instruction(instruction);
	FILE *fcbPointer  = fopen(get_fcb_path(instruction), "w");
	persist_fcb(fcbPointer,fcb);
	log_fcb(fcb);
	fclose(fcbPointer);
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

	FILE *filePointer  = fopen(get_fcb_path(instruction), "r");
	if(filePointer == NULL)
		return OPERATION_RESULT_ERROR;

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("Abrir Archivo: %s",
				list_get(instruction->parameters, 0))
	);

	fclose(filePointer);
	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_read(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_read] Ejecutando F_READ");
	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_write(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_write] Ejecutando F_WRITE");
	return OPERATION_RESULT_OK;
}

operation_result execute_fs_f_truncate(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_truncate] Ejecutando F_TRUNCATE");
	return OPERATION_RESULT_OK;
}


char* get_file_path(t_instruction* instruction){
	char* filePath = "./files/";
	char* fileName = list_get(instruction->parameters, 0);
	char* fileExtension = ".dat";

	// Obtener la longitud total de la cadena resultante
	size_t totalLength = strlen(filePath) + strlen(fileName) + strlen(fileExtension) + 1;

	// Asignar memoria para la cadena resultante
	char* filePathResult = malloc(totalLength);

	// Copiar las partes de la cadena en la memoria asignada
	strcpy(filePathResult, filePath);
	strcat(filePathResult, fileName);
	strcat(filePathResult, fileExtension);

	return filePathResult;
}



