#include "commands-utils.h"

int execute_set(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/commands-utils - execute_set] Ejecutando SET");

	sleep(get_cpu_env()->INSTRUCTION_DELAY);

	t_instruction* currentInstruction = list_get(context->instructions, context->programCounter);

	char* reg = list_get(currentInstruction->parameters,0);
	char* value = list_get(currentInstruction->parameters,1);

	if(strcmp(reg,"AX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register AX");
		strncpy(context->cpuRegisters->AX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"BX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register BX");
		strncpy(context->cpuRegisters->BX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"CX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register CX");
		strncpy(context->cpuRegisters->CX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"DX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register DX");
		strncpy(context->cpuRegisters->DX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"EAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register EAX");
		strncpy(context->cpuRegisters->EAX, value, sizeof(char)*8);
		return 1;

	}

	if(strcmp(reg,"EBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register EBX");
		strncpy(context->cpuRegisters->EBX, value, sizeof(char)*8);
		return 1;
	}

	if(strcmp(reg,"ECX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register ECX");
		strncpy(context->cpuRegisters->ECX, value, sizeof(char)*8);
		return 1;
	}

	if(strcmp(reg,"EDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register EDX");
		strncpy(context->cpuRegisters->EDX, value, sizeof(char)*8);
		return 1;
	}

	if(strcmp(reg,"RAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register RAX");
		strncpy(context->cpuRegisters->RAX, value, sizeof(char)*16);
		return 1;
	}

	if(strcmp(reg,"RBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register RBX");
		strncpy(context->cpuRegisters->RBX, value, sizeof(char)*16);
		return 1;
	}

	if(strcmp(reg,"RCX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register RCX");
		strncpy(context->cpuRegisters->RCX, value, sizeof(char)*16);
		return 1;
	}

	if(strcmp(reg,"RDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/commands-utils - execute_set] Asignando Register RDX");
		strncpy(context->cpuRegisters->RDX, value, sizeof(char)*16);
		return 1;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/commands-utils - execute_set] Registro no valido");

	return 0;
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
	return 0;
}

int execute_mov_out(t_execution_context* context){
	return 0;
}

int execute_f_open(t_execution_context* context){
	return 0;
}

int execute_f_close(t_execution_context* context){
	return 0;
}

int execute_f_seek(t_execution_context* context){
	return 0;
}

int execute_f_read(t_execution_context* context){
	return 0;
}

int execute_f_write(t_execution_context* context){
	return 0;
}

int execute_f_truncate(t_execution_context* context){
	return 0;
}

int execute_create_segment(t_execution_context* context){
	return 0;
}

int execute_delete_segment(t_execution_context* context){
	return 0;
}
void pass_instructions_parameters_to_reason_parameters(t_instruction* currentInstruction, t_list* reasonParameters){
	int size = list_size(currentInstruction->parameters);

	for(int i=0;i<size;i++){
		char* value = list_get(currentInstruction->parameters,i);
		list_clean(reasonParameters);
		list_add(reasonParameters, value);
	}
}
