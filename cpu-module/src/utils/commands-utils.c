#include "commands-utils.h"

int execute_set(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_set] Ejecutando SET");

	sleep(get_cpu_env()->INSTRUCTION_DELAY);

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);

	char* reg = list_get(currentInstruction->parameters,0);
	char* value = list_get(currentInstruction->parameters,1);

	return set_register_value(reg, value, context);
}

int set_register_value(char* reg, char* value, t_execution_context* context){
	int size = get_amount_of_bytes_per_register(reg, context);
	value[size] = '\0';
	if(strcmp(reg,"AX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register AX");
		strncpy(context->cpuRegisters->AX, value, size);
		return 1;
	}

	if(strcmp(reg,"BX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register BX");
		strncpy(context->cpuRegisters->BX, value, size);
		return 1;
	}

	if(strcmp(reg,"CX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register CX");
		strncpy(context->cpuRegisters->CX, value, size);
		return 1;
	}

	if(strcmp(reg,"DX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register DX");
		strncpy(context->cpuRegisters->DX, value, size);
		return 1;
	}

	if(strcmp(reg,"EAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register EAX");
		strncpy(context->cpuRegisters->EAX, value, size);
		return 1;

	}

	if(strcmp(reg,"EBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register EBX");
		strncpy(context->cpuRegisters->EBX, value, size);
		return 1;
	}

	if(strcmp(reg,"ECX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register ECX");
		strncpy(context->cpuRegisters->ECX, value, size);
		return 1;
	}

	if(strcmp(reg,"EDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register EDX");
		strncpy(context->cpuRegisters->EDX, value, size);
		return 1;
	}

	if(strcmp(reg,"RAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register RAX");
		strncpy(context->cpuRegisters->RAX, value, size);
		return 1;
	}

	if(strcmp(reg,"RBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register RBX");
		strncpy(context->cpuRegisters->RBX, value, size);
		return 1;
	}

	if(strcmp(reg,"RCX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register RCX");
		strncpy(context->cpuRegisters->RCX, value, size);
		return 1;
	}

	if(strcmp(reg,"RDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - set_register_value] Asignando Register RDX");
		strncpy(context->cpuRegisters->RDX, value, size);
		return 1;
	}

	return 0;
}

char* get_register_value(char* reg, t_execution_context* context){
	//Esto deberia devolver 4/8/16 bytes tal cual estén;
	int length = get_amount_of_bytes_per_register(reg, context) + 1;
	char* partialStr = malloc(length);

	char* value;

	if(strcmp(reg,"AX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register AX");
		value = context->cpuRegisters->AX;
	}

	if(strcmp(reg,"BX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register BX");
		value = context->cpuRegisters->BX;
	}

	if(strcmp(reg,"CX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register CX");
		value = context->cpuRegisters->CX;
	}

	if(strcmp(reg,"DX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register DX");
		value = context->cpuRegisters->DX;
	}

	if(strcmp(reg,"EAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register EAX");
		value = context->cpuRegisters->EAX;
	}

	if(strcmp(reg,"EBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register EBX");
		value = context->cpuRegisters->EBX;
	}

	if(strcmp(reg,"ECX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register ECX");
		value = context->cpuRegisters->ECX;
	}

	if(strcmp(reg,"EDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register EDX");
		value = context->cpuRegisters->EDX;
	}

	if(strcmp(reg,"RAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register RAX");
		value = context->cpuRegisters->RAX;
	}

	if(strcmp(reg,"RBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register RBX");
		value = context->cpuRegisters->RBX;
	}

	if(strcmp(reg,"RCX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register RCX");
		value = context->cpuRegisters->RCX;
	}

	if(strcmp(reg,"RDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - get_register_value] Obteniendo Register RDX");
		value = context->cpuRegisters->RDX;
	}
	strncpy(partialStr, value, length);
	partialStr[length] = '\0';  // Null-terminate the string

	return partialStr;
}

int execute_yield(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_yield] Ejecutando YIELD");
	context->reason->executionContextState = REASON_YIELD;
	return 0;
}

int execute_exit(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_exit] Ejecutando EXIT");
	context->reason->executionContextState = REASON_FINISH;
	return 0;
}

int execute_wait(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_wait] Ejecutando WAIT");
	context->reason->executionContextState = REASON_WAIT;

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);

	pass_instructions_parameters_to_reason_parameters(currentInstruction,context->reason->parameters);

	return 0;
}


int execute_signal(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_signal] Ejecutando SIGNAL");
	context->reason->executionContextState = REASON_SIGNAL;

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);

	pass_instructions_parameters_to_reason_parameters(currentInstruction,context->reason->parameters);

	return 0;
}

int execute_io(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_signal] Ejecutando I/O");
	context->reason->executionContextState = REASON_IO;

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);

	pass_instructions_parameters_to_reason_parameters(currentInstruction,context->reason->parameters);

	return 0;
}

int execute_mov_in(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_mov_in] Ejecutando MOV_IN");

	t_instruction* instruction = list_get(context->instructions, context->programCounter);
	char* reg = list_get(instruction->parameters, 0);
	int regSize = get_amount_of_bytes_per_register(reg, context);

	int physicalAddress = get_physical_address(get_logger(), context,regSize, list_get(instruction->parameters, 1), get_cpu_env()->SEGMENT_MAX_SIZE);
	if (physicalAddress == -1){
		list_clean(context->reason->parameters);
		list_add(context->reason->parameters, error_as_string(SEG_FAULT));
		context->reason->executionContextState = REASON_ERROR;
		return 0;
	}

	send_mov_in_to_memory(context, regSize, physicalAddress);

	handle_mov_in_memory_response(context);
	//return error o algo si sale mal
	return 1;
}

int execute_mov_out(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_mov_out] Ejecutando MOV_OUT");

	t_instruction* instruction = list_get(context->instructions, context->programCounter);
	char* reg = list_get(instruction->parameters, 1);
	int regSize = get_amount_of_bytes_per_register(reg, context);

	int physicalAddress = get_physical_address(get_logger(), context, regSize, list_get(instruction->parameters, 0), get_cpu_env()->SEGMENT_MAX_SIZE);

	if (physicalAddress == -1){
		list_clean(context->reason->parameters);
		list_add(context->reason->parameters, error_as_string(SEG_FAULT));
		context->reason->executionContextState = REASON_ERROR;
		return 0;
	}

	send_mov_out_to_memory(context,reg, physicalAddress);

	// TODO: Hace falta hacer algo con response ademas de un operation result?
	operation_result response;
	recv(get_memory_connection(), &response, sizeof(int), MSG_WAITALL);

	return 1;
}

int execute_f_open(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_f_open] Ejecutando F_OPEN");
	context->reason->executionContextState = REASON_F_OPEN;
	return 0;
}

int execute_f_close(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_f_close] Ejecutando F_CLOSE");
	context->reason->executionContextState = REASON_F_CLOSE;
	return 0;
}

int execute_f_seek(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_f_seek] Ejecutando F_SEEK");
	context->reason->executionContextState = REASON_F_SEEK;
	return 0;
}

int execute_f_read(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_f_read] Ejecutando F_READ");
	context->reason->executionContextState = REASON_F_READ;
	return 0;
}

int execute_f_write(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_f_write] Ejecutando F_WRITE");
	context->reason->executionContextState = REASON_F_WRITE;
	return 0;
}

int execute_f_truncate(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_f_truncate] Ejecutando F_TRUNCATE");
	context->reason->executionContextState = REASON_F_TRUNCATE;
	return 0;
}

int execute_create_segment(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_create_segment] Ejecutando CREATE_SEGMENT");
	context->reason->executionContextState = REASON_CREATE_SEGMENT;

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);
	pass_instructions_parameters_to_reason_parameters(currentInstruction, context->reason->parameters);

	return 0;
}

int execute_delete_segment(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_delete_segment] Ejecutando DELETE_SEGMENT");
	context->reason->executionContextState = REASON_DELETE_SEGMENT;

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);
	pass_instructions_parameters_to_reason_parameters(currentInstruction, context->reason->parameters);
	return 0;
}

void pass_instructions_parameters_to_reason_parameters(t_instruction* currentInstruction, t_list* reasonParameters){
	int size = list_size(currentInstruction->parameters);
	list_clean(reasonParameters);

	for(int i=0;i<size;i++){
		char* value = list_get(currentInstruction->parameters,i);
		list_add(reasonParameters, value);
	}
}
