#include "pcb-utils.h"

t_pcb* new(void){
	t_pcb *pcb = malloc(sizeof(t_pcb));

	pcb->instructions = list_create();
	pcb->segment_table = list_create();
	pcb->open_files_table = list_create();
	pcb->state=NEW;

	return pcb;
}

t_pcb* create_pcb(t_list* intructions){
	t_pcb *pcb = new();

	pcb->instructions=intructions;

	return pcb;
}

t_list* create_instruction_list(t_list* userInput){
	t_list* instructions = list_create();

	for (int i=0; i< list_size(userInput);i++){
		t_kernel_instruction * inst = create_instruction(list_get(userInput,i));
		list_add(instructions, inst);
	}

	return instructions;
}

t_kernel_instruction* create_instruction(char* instruction){
	t_kernel_instruction * inst = malloc(sizeof(t_kernel_instruction));
	inst->parameters = list_create();

	char** split = string_split(instruction," ");

	inst->command= kernel_command_from_string(split[0]);
	for (int i=1; i< string_array_size(split);i++){
		list_add(inst->parameters, split[i]);
	}

	return inst;
}


void add_intruction(t_pcb* pcb, t_kernel_instruction* instruction){
	list_add(pcb->instructions,instruction);
}

void add_segment(t_pcb* pcb, t_segment_row* segment){
	list_add(pcb->segment_table,segment);
}

void add_file(t_pcb* pcb, t_open_file_row* openFile){
	list_add(pcb->open_files_table,openFile);
}

void free_pcb(t_pcb* pcb){
	//TODO: crear funcion que destruya elementos
	//list_destroy_and_destroy_elements(pcb,funcionDestructiva)
}

void write_intructions_to_all_logs(t_kernel_instruction* instruction) {

	write_info_to_all_logs(kernel_command_as_string(instruction->command));
	list_iterate(instruction->parameters, (void*) write_info_to_all_logs);
}


kernel_command kernel_command_from_string(char * command) {
	int i;

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Ejecutando kernel_command_from_string, parametros : %s",command));

	for (i = 0; i < COMMAND_ENUM_SIZE; i++) {
		if (string_equals_ignore_case(command, commandNames[i])){
			return i;
		}
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, string_from_format("Command no se corresponde a un valor existente"));

	return -1;
}

char * kernel_command_as_string(kernel_command command) {
	return commandNames[command];
}

