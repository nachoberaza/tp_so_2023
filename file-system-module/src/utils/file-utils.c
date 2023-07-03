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
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_INFO,
		string_from_format("Se encontro el fcb con nombre: %s",
				fileName)
	);
	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("Abrir Archivo: %s",
				fileName)
	);

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

operation_result execute_fs_f_seek(t_instruction* instruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - execute_fs_f_seek] Ejecutando F_SEEK");
	return OPERATION_RESULT_OK;
}


