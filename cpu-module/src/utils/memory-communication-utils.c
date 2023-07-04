#include "memory-communication-utils.h"

void send_mov_in_to_memory(t_execution_context* context, int size, int physicalAddress){
	t_package* package = create_package();
	t_instruction* memoryInstruction = malloc(sizeof(t_instruction));

	memoryInstruction->command = MOV_IN;
	memoryInstruction->parameters = list_create();
	list_add(memoryInstruction->parameters, string_itoa(physicalAddress));
	list_add(memoryInstruction->parameters, string_itoa(size));

	write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, memoryInstruction);

	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = context->pid;
	memoryData->instruction = memoryInstruction;


	fill_buffer_with_memory_data(memoryData,package);

	send_package(package, get_memory_connection());
}

void send_mov_out_to_memory(t_execution_context* context, char* value, int size,int physicalAddress){
	t_package* package = create_package();

	t_instruction* memoryInstruction = malloc(sizeof(t_instruction));
	memoryInstruction->command = MOV_OUT;

	memoryInstruction->parameters = list_create();
	list_add(memoryInstruction->parameters, string_itoa(physicalAddress));
	list_add(memoryInstruction->parameters, value);

	//Lo necesitamos para el log de memory
	list_add(memoryInstruction->parameters, string_itoa(size));

	write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, memoryInstruction);

	t_memory_data* memoryData = malloc(sizeof(t_memory_data));
	memoryData->pid = context->pid;
	memoryData->instruction = memoryInstruction;

	fill_buffer_with_memory_data(memoryData,package);

	send_package(package, get_memory_connection());
}

char* handle_mov_in_memory_response(t_execution_context* context){
	t_instruction* instruction = list_get(context->instructions, context->programCounter);
	char* reg = list_get(instruction->parameters, 0);
	int bufferSize, offset = 0;
	void *buffer;

	receive_operation_code(get_memory_connection());
	buffer = receive_buffer(&bufferSize, get_memory_connection());
	char* value = extract_string_from_buffer(buffer, &offset);

	set_register_value(reg, value, context);

	//lo retorno para el log
	return value;
}
