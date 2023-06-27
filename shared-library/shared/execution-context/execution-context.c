#include "execution-context.h"

t_execution_context* init_execution_context(int pid){
	t_execution_context* executionContext = malloc(sizeof(t_execution_context));
	executionContext->pid = pid;
	executionContext->cpuRegisters = malloc(sizeof(t_cpu_register));
	executionContext->instructions = list_create();
	executionContext->programCounter = 0;
	executionContext->reason = malloc(sizeof(t_execution_context_reason));
	executionContext->reason->parameters = list_create();
	executionContext->segmentTable = list_create();

	return executionContext;
}

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

	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, "[shared/execution-context - log_context] Segment table:");
	list_iterate(context->segmentTable, (void*) write_segment_row_to_internal_logs);
}

void write_segment_row_to_internal_logs(t_segment_row* segmentRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("Pid: %d", segmentRow->pid));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("Id: %d", segmentRow->id));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("BaseDirection: %d", segmentRow->baseDirection));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("SegmentSize: %d \n", segmentRow->segmentSize));
}

void write_instructions_to_internal_logs(t_log_grouping* logger, t_log_level logLevel, t_list* instructions) {
	for (int i=0; i < list_size(instructions); i++){
		t_instruction * instruction = list_get(instructions, i);
		write_instruction_to_internal_log(logger, logLevel, instruction);
	}
}

void write_instruction_to_internal_log(t_log_grouping* logger, t_log_level logLevel, t_instruction* instruction) {
	write_log_grouping(logger, LOG_TARGET_INTERNAL, logLevel, command_as_string(instruction->command));

	for (int i=0; i < list_size(instruction->parameters); i++){
		write_parameter_to_internal_logs(logger, logLevel, list_get(instruction->parameters, i));
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

	list_destroy_and_destroy_elements(executionContext->segmentTable, (void*) destroy_segment_row);
	destroy_instructions(logger ,executionContext->instructions);
	destroy_execution_context_reason(logger ,executionContext->reason);

	free(executionContext);
}

void destroy_segment_row(t_segment_row* segmentRow) {
	free(segmentRow);
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

int get_amount_of_bytes_per_register(char* reg, t_execution_context* context){
	if(strcmp(reg,"AX") == 0){
		return 4;
	}

	if(strcmp(reg,"BX") == 0){
		return 4;
	}

	if(strcmp(reg,"CX") == 0){
		return 4;
	}

	if(strcmp(reg,"DX") == 0){
		return 4;
	}

	if(strcmp(reg,"EAX") == 0){
		return 8;
	}

	if(strcmp(reg,"EBX") == 0){
		return 8;
	}

	if(strcmp(reg,"ECX") == 0){
		return 8;
	}

	if(strcmp(reg,"EDX") == 0){
		return 8;
	}

	if(strcmp(reg,"RAX") == 0){
		return 16;
	}

	if(strcmp(reg,"RBX") == 0){
		return 16;
	}

	if(strcmp(reg,"RCX") == 0){
		return 16;
	}

	if(strcmp(reg,"RDX") == 0){
		return 16;
	}

	return 0;
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
	context->segmentTable = extract_segment_table_from_buffer(buffer, &offset);

	free(buffer);

	return context;
}

t_list* extract_segment_table_from_buffer(void* buffer, int *offset){
	t_list* segmentTable = list_create();
	int segmentTableCount = extract_int_from_buffer(buffer, offset);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_DEBUG,
		string_from_format("Recibí tabla con %d elementos", segmentTableCount)
	);

	for (int i = 0; i < segmentTableCount; i++){
		t_segment_row* segmentRow = malloc(sizeof(t_segment_row));

		segmentRow->pid = extract_int_from_buffer(buffer, offset);
		segmentRow->id = extract_int_from_buffer(buffer, offset);
		segmentRow->baseDirection = extract_int_from_buffer(buffer, offset);
		segmentRow->segmentSize = extract_int_from_buffer(buffer, offset);
		list_add(segmentTable, segmentRow);

	}

	return segmentTable;
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
		t_instruction* instruction = extract_instruction_from_buffer(logger, logLevel, buffer, offset);
		list_add(instructions, instruction);
	}

	return instructions;
}

t_instruction* extract_instruction_from_buffer(t_log_grouping* logger, t_log_level logLevel, void * buffer, int* offset){
	t_instruction* instruction = malloc(sizeof(t_instruction));
	instruction->parameters = list_create();

	instruction->command = extract_command_from_buffer(buffer, offset);

	extract_instruction_parameters_from_buffer(instruction->parameters, buffer, offset);

	return instruction;
}

void extract_instruction_parameters_from_buffer(t_list* parameters, void * buffer, int* offset) {
	int parameterCount = extract_int_from_buffer(buffer, offset);

	for (int j = 0; j < parameterCount; j++){
		char* value = extract_string_from_buffer(buffer, offset);
		list_add(parameters, value);
	}
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

void fill_package_with_segment_table(t_package* pkg, t_list* segmentTable){
	int segmentTableCount = list_size(segmentTable);
	fill_package_buffer(pkg, &segmentTableCount, sizeof(int));

	for (int i = 0; i < segmentTableCount; i++){
		t_segment_row* segment = list_get(segmentTable, i);

		fill_package_buffer(pkg, &segment->pid, sizeof(int));
		fill_package_buffer(pkg, &segment->id, sizeof(int));
		fill_package_buffer(pkg, &segment->baseDirection, sizeof(int));
		fill_package_buffer(pkg, &segment->segmentSize, sizeof(int));
	}
}

void fill_package_with_context(t_log_grouping* logger,t_execution_context* context, t_package* pkg){
	//Asigno valores porque si llega "0" no se si esta bien o si llegó cualquier falopa

	//Paso paquete, puntero a variable que quiero se envie y el tamaño de la variable
	fill_package_buffer(pkg, &(context->pid), sizeof(int));
	fill_package_buffer(pkg, &(context->programCounter), sizeof(int));

	fill_buffer_with_execution_context_reason(logger,context->reason,pkg);
	fill_buffer_with_instructions(logger,context->instructions,pkg);
	fill_buffer_with_cpu_register(context->cpuRegisters, pkg);
	fill_package_with_segment_table(pkg, context->segmentTable);
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

	for (int i = 0; i < instructionsCount; i++){
		t_instruction* instruction = list_get(instructions, i);
		fill_buffer_with_instruction(instruction, pkg);
	}
}

void fill_buffer_with_instruction(t_instruction* instruction, t_package* pkg){
	fill_package_buffer(pkg, &(instruction->command), sizeof(command));

	int parameterCount = list_size(instruction->parameters);

	fill_package_buffer(pkg, &parameterCount, sizeof(int));
	for (int j = 0; j < parameterCount; j++){
		char* parameter = list_get(instruction->parameters, j);

		fill_package_buffer(pkg, parameter, strlen(parameter) + 1);
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


error error_from_string(t_log_grouping* logger, char * error) {
	write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("[shared/execution-context - error_from_string] Error a buscar : %s",error));

	for (int i = 0; i < ERROR_ENUM_SIZE; i++) {
		if (string_equals_ignore_case(error, errorNames[i])){
			write_log_grouping(logger,LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "[shared/execution-context - error_from_string] Error Name encontrado");
			return i;
		}
	}
	write_log_grouping(logger, LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, string_from_format("[shared/execution-context - error_from_string] El Error no se corresponde a un valor existente"));

	return -1;
}


char * error_as_string(error error) {
	return errorNames[error];
}

