#include "kernel-module.h"

int main(void) {
	t_kernel_config *KERNEL_ENV = create_kernel_config(MODULE_NAME);
	init_logger(MODULE_NAME, KERNEL_ENV->LOG_LEVEL);

	log_config(KERNEL_ENV);

	int serverSocketId = start_server(KERNEL_ENV->IP, KERNEL_ENV->PORT, get_logger());
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Servidor de kernel listo");

	t_kernel_connections* KERNEL_CONNECTIONS = start_connections(KERNEL_ENV);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Kernel conectado a CPU, FileSystem y Memoria");
	int clientSocketId = await_client(get_logger(), serverSocketId);

	t_list *commands;
	while (1) {
		int operationCode = receive_operation_code(clientSocketId);
		switch (operationCode) {
		case MESSAGE:
			decode_message(get_logger(),clientSocketId);
			break;
		case PACKAGE:
			commands = decode_package(clientSocketId);
			t_list* instructions= create_instruction_list(commands);
			t_pcb* pcb = create_pcb(instructions);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Instrucciones en el PCB:\n");
			list_iterate(pcb->instructions, (void*) write_intructions_to_all_logs);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Me llego el siguiente package:\n");
			list_iterate(commands, (void*) write_info_to_all_logs);
			t_package* packageReceived = build_package(commands);
			send_package(packageReceived, KERNEL_CONNECTIONS->cpu);
			send_package(packageReceived, KERNEL_CONNECTIONS->memory);
			send_package(packageReceived, KERNEL_CONNECTIONS->fileSystem);
			delete_package(packageReceived);
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Mande valores a FileSystem, CPU y MEMORIA!\n");
			break;
		case -1:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "La consola se desconecto. Cerrando servidor");
			return EXIT_FAILURE;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING, "Operacion desconocida.");
			break;
		}
	}

	return EXIT_SUCCESS;
}


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
	inst->command= SET;//cambiar
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

	write_info_to_all_logs("SET");
	list_iterate(instruction->parameters, (void*) write_info_to_all_logs);
}

