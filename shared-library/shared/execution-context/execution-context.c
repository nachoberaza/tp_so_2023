#include "execution-context.h"

void log_context(t_log_grouping* logger, t_log_level logLevel, t_execution_context* context) {

	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, "[shared/execution-context - log_context] Logeando contexto:\n");
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, string_from_format("pid: %d", context->pid));
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, string_from_format("Program counter: %d \n", context->programCounter));

	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, "[shared/execution-context - log_context] Reason del contexto:");
	write_execution_context_reason_to_internal_logs(logger, logLevel, context->reason);

	t_cpu_register* cpu_register = context->cpuRegisters;
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, "[shared/execution-context - log_context] Registros del contexto:");
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, string_from_format("AX: %s, BX: %s, CX: %s, DX: %s", cpu_register->AX, cpu_register->BX, cpu_register->CX, cpu_register->DX));
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel,  string_from_format("EAX: %s, EBX: %s, ECX: %s, EDX: %s", cpu_register->EAX, cpu_register->EBX, cpu_register->ECX, cpu_register->EDX));
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, string_from_format("RAX: %s, RBX: %s, RCX: %s, RDX: %s \n", cpu_register->RAX, cpu_register->RBX, cpu_register->RCX, cpu_register->RDX));

	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, "[shared/execution-context - log_context] Instrucciones del contexto:");
	write_instructions_to_internal_logs(logger, logLevel, context->instructions);

}

void write_instructions_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, t_list* instructions) {
	for (int i=0; i < list_size(instructions); i++){
		t_instruction * instruction = list_get(instructions, i);
		write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, command_as_string(instruction->command));

		for (int i=0; i < list_size(instruction->parameters); i++){
			write_parameter_to_internal_logs(logger, logLevel, list_get(instruction->parameters, i));
		}
	}
}

void write_parameter_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, char* parameter) {
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, string_from_format("Parameter: %s", parameter));
}

void write_execution_context_reason_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, t_execution_context_reason* reason) {
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, execution_context_state_as_string(reason->executionContextState));

	for (int i=0; i < list_size(reason->parameters); i++){
		write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, list_get(reason->parameters, i));
	}
}


void destroy_instruction(t_instruction* instruction) {
	list_destroy(instruction->parameters);
	free(instruction);
}

void destroy_instructions(t_log_grouping* logger,t_list* instructions) {
	list_destroy_and_destroy_elements(instructions, (void*) destroy_instruction);

	write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[shared/execution-context - destroy_instructions] Instructiones liberadas");
}

void destroy_execution_context_reason(t_log_grouping* logger, t_execution_context_reason* reason) {
	list_destroy(reason->parameters);

	free(reason);
	write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[shared/execution-context - destroy_execution_context_reason] Reason liberado");
}


void destroy_execution_context(t_log_grouping* logger, t_execution_context* executionContext) {
	write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[shared/execution-context - destroy_execution_context] Ejecutando destroy_execution_context");

	free(executionContext->cpuRegisters);
	write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[shared/execution-context - destroy_execution_context] CPU Registers liberados");

	destroy_instructions(logger ,executionContext->instructions);
	destroy_execution_context_reason(logger ,executionContext->reason);

	free(executionContext);
}

command command_from_string(t_log_grouping* logger, char * command) {
	write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("[shared/execution-context - command_from_string] Comando a buscar : %s",command));

	for (int i = 0; i < COMMAND_ENUM_SIZE; i++) {
		if (string_equals_ignore_case(command, commandNames[i])){
			write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[shared/execution-context - command_from_string] Comando encontrado");
			return i;
		}
	}
	write_log_grouping(logger, LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, string_from_format("[shared/execution-context - command_from_string] El comando no se corresponde a un valor existente"));

	return -1;
}


char * command_as_string(command command) {
	return commandNames[command];
}

char * execution_context_state_as_string(execution_context_state executionContextState) {
	return executionContextStateNames[executionContextState];
}


t_execution_context* decode_context(t_log_grouping* logger, t_log_level logLevel, int clientSocket) {
	int bufferSize, offset = 0;
	void *buffer;

	t_execution_context* context = malloc(sizeof(t_execution_context));

	buffer = receive_buffer(&bufferSize, clientSocket);

	context->pid = extract_int_from_buffer(buffer, &offset);
	context->programCounter = extract_int_from_buffer(buffer, &offset);
	context->reason = extract_execution_context_reason_from_buffer(buffer, &offset);
	context->instructions = extract_instructions_from_buffer(logger, logLevel, buffer, &offset);
	context->cpuRegisters = extract_cpu_register_from_buffer(buffer, &offset);

	free(buffer);

	return context;
}

t_execution_context_reason* extract_execution_context_reason_from_buffer(void * buffer, int* offset){

	t_execution_context_reason* reason = malloc(sizeof(t_execution_context_reason));

	reason->parameters = list_create();

	reason->executionContextState = extract_execution_context_state_from_buffer(buffer, offset);

	int parametersCount = extract_int_from_buffer(buffer, offset);

	for (int j = 0; j < parametersCount; j++){
		char* value = extract_string_from_buffer(buffer, offset);
		list_add(reason->parameters, value);
	}

	return reason;
}


execution_context_state extract_execution_context_state_from_buffer(void* buffer, int* offset){
	int valueSize = 0;
	execution_context_state returnValue;
	memcpy(&valueSize, buffer + *offset, sizeof(int));
	*offset += sizeof(int);
	memcpy(&returnValue, buffer + *offset, sizeof(execution_context_state));
	*offset += sizeof(int);

	return returnValue;
}


t_list* extract_instructions_from_buffer(t_log_grouping* logger, t_log_level logLevel, void * buffer, int* offset){

	t_list* instructions = list_create();

	int instructionsCount = extract_int_from_buffer(buffer, offset);
	write_log_grouping(
		logger,
		LOG_TARGET_INTERNAL,
		logLevel,
		string_from_format("[shared/execution-context - extract_instructions_from_buffer] Cantidad de instrucciones: %d", instructionsCount)
	);

	for (int i = 0; i < instructionsCount; i++){
		t_instruction* instruction = malloc(sizeof(t_instruction));
		instruction->parameters = list_create();

		instruction->command = extract_command_from_buffer(buffer, offset);

		int parameterCount = extract_int_from_buffer(buffer, offset);

		for (int j = 0; j < parameterCount; j++){
			char* value = extract_string_from_buffer(buffer, offset);
			list_add(instruction->parameters, value);
		}

		list_add(instructions, instruction);
	}

	return instructions;
}

t_cpu_register* extract_cpu_register_from_buffer(void* buffer, int* offset){
	t_cpu_register* cpuRegister = malloc(sizeof(t_cpu_register));

	copy_string_from_buffer_into_variable(cpuRegister->AX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->BX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->CX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->DX, buffer, offset);

	copy_string_from_buffer_into_variable(cpuRegister->EAX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->EBX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->ECX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->EDX, buffer, offset);

	copy_string_from_buffer_into_variable(cpuRegister->RAX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->RBX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->RCX, buffer, offset);
	copy_string_from_buffer_into_variable(cpuRegister->RDX, buffer, offset);

	return cpuRegister;
}

void fill_package_with_context(t_log_grouping* logger,t_execution_context* context, t_package* pkg){
	//Asigno valores porque si llega "0" no se si esta bien o si llegó cualquier falopa

	//Paso paquete, puntero a variable que quiero se envie y el tamaño de la variable
	fill_package_buffer(pkg, &(context->pid), sizeof(int));
	fill_package_buffer(pkg, &(context->programCounter), sizeof(int));

	fill_buffer_with_execution_context_reason(logger,context->reason,pkg);
	fill_buffer_with_instructions(logger,context->instructions,pkg);
	fill_buffer_with_cpu_register(context->cpuRegisters, pkg);
}

void fill_buffer_with_execution_context_reason(t_log_grouping* logger,t_execution_context_reason* reason, t_package* pkg){
	//Para listas, el primer número va a ser la cant de elementos

	fill_package_buffer(pkg, &(reason->executionContextState), sizeof(execution_context_state));


	int parameterCount = list_size(reason->parameters);
	fill_package_buffer(pkg, &parameterCount, sizeof(int));

	for (int j = 0; j < parameterCount; j++){
		char* parameter = list_get(reason->parameters, j);

		fill_package_buffer(pkg, parameter, strlen(parameter) + 1);
	}
}

void fill_buffer_with_instructions(t_log_grouping* logger,t_list* instructions, t_package* pkg){
	//Para listas, el primer número va a ser la cant de elementos
	int instructionsCount = list_size(instructions);
	fill_package_buffer(pkg, &instructionsCount, sizeof(int));
	write_log_grouping(
		logger,
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		string_from_format("[shared/execution-context - fill_buffer_with_instructions] Cantidad de instrucciones: %d", instructionsCount)
	);

	//Por cada elemento de la lista envio el elemento, no mando al buffer &struct
	//porque trae problemas, ej en este caso el struct lo que tiene es un puntero
	//a una lista, entonces mando cada parte del elemento por separado y del otro lado
	//asumo que se sabe que cada elemento se corresponde a una parte de la struct
	for (int i = 0; i < instructionsCount; i++){
		t_instruction* instruction = list_get(instructions, i);
		fill_package_buffer(pkg, &(instruction->command), sizeof(command));

		int parameterCount = list_size(instruction->parameters);

		fill_package_buffer(pkg, &parameterCount, sizeof(int));
		for (int j = 0; j < parameterCount; j++){
			char* parameter = list_get(instruction->parameters, j);

			fill_package_buffer(pkg, parameter, strlen(parameter) + 1);
		}
	}
}

void fill_buffer_with_cpu_register(t_cpu_register* cpuRegisters, t_package* pkg){
	fill_package_buffer(pkg, cpuRegisters->AX, sizeof(char) * 4);
	fill_package_buffer(pkg, cpuRegisters->BX, sizeof(char) * 4);
	fill_package_buffer(pkg, cpuRegisters->CX, sizeof(char) * 4);
	fill_package_buffer(pkg, cpuRegisters->DX, sizeof(char) * 4);

	fill_package_buffer(pkg, cpuRegisters->EAX, sizeof(char) * 8);
	fill_package_buffer(pkg, cpuRegisters->EBX, sizeof(char) * 8);
	fill_package_buffer(pkg, cpuRegisters->ECX, sizeof(char) * 8);
	fill_package_buffer(pkg, cpuRegisters->EDX, sizeof(char) * 8);

	fill_package_buffer(pkg, cpuRegisters->RAX, sizeof(char) * 16);
	fill_package_buffer(pkg, cpuRegisters->RBX, sizeof(char) * 16);
	fill_package_buffer(pkg, cpuRegisters->RCX, sizeof(char) * 16);
	fill_package_buffer(pkg, cpuRegisters->RDX, sizeof(char) * 16);
}

