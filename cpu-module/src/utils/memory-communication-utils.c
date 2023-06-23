#include "memory-communication-utils.h"

void send_mov_in_to_memory(t_execution_context* context, char* reg, int physicalAddress){
	t_instruction* instruction = list_get(context->instructions, context->programCounter);

	t_package* package = create_package();

	int size = get_amount_of_bytes_per_register(reg, context);

	t_instruction* memoryInstruction = malloc(sizeof(t_instruction));

	memoryInstruction->parameters = list_create();
	memoryInstruction->command = MOV_IN;
	list_add(memoryInstruction->parameters, string_itoa(physicalAddress));
	list_add(memoryInstruction->parameters, string_itoa(size));
	write_instruction_to_internal_log(get_logger(), LOG_LEVEL_INFO, memoryInstruction);

	fill_buffer_with_instruction(memoryInstruction, package);
	send_package(package, get_memory_connection());

}

void send_mov_out_to_memory(t_execution_context* context, int physicalAddress){
	t_instruction* instruction = list_get(context->instructions, context->programCounter);
	t_package* package = create_package();

	char* reg = list_get(instruction->parameters, 1);

	char* value = get_register_value(reg, context);

	t_instruction* memoryInstruction = malloc(sizeof(t_instruction));
	memoryInstruction->parameters = list_create();

	memoryInstruction->command = MOV_OUT;
	list_add(memoryInstruction->parameters, string_itoa(physicalAddress));
	list_add(memoryInstruction->parameters, value);

	fill_buffer_with_instruction(memoryInstruction, package);
	send_package(package, get_memory_connection());
}

void handle_mov_in_memory_response(t_execution_context* context){
	t_instruction* instruction = list_get(context->instructions, context->programCounter);
	char* reg = list_get(instruction->parameters, 0);
	int bufferSize, offset = 0;
	void *buffer;

	receive_operation_code(get_memory_connection());
	buffer = receive_buffer(&bufferSize, get_memory_connection());
	char* value = extract_string_from_buffer(buffer, &offset);

	set_register_value(reg, value, context);
}
