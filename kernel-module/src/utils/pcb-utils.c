#include "pcb-utils.h"

int currentPid = 0;

t_pcb* new_pcb(void){
	t_pcb *pcb = malloc(sizeof(t_pcb));

	pcb->pid = get_pid();
	pcb->instructions = list_create();
	pcb->segmentTable = list_create();
	pcb->openFilesTable = list_create();
	pcb->cpuRegisters = malloc(sizeof(t_kernel_instruction));
	pcb->state = NEW;

	return pcb;
}

t_pcb* create_pcb_from_lines(t_list* lines){
	t_pcb *pcb = new_pcb();

	populate_instruction_list_from_lines(pcb->instructions,lines);

	return pcb;
}

void populate_instruction_list_from_lines(t_list* instructions,t_list* lines){
	for (int i=0; i < list_size(lines); i++){
		t_kernel_instruction * instruction = create_instruction(list_get(lines, i));
		list_add(instructions, instruction);
	}
}

t_kernel_instruction* create_instruction(char* line){
	t_kernel_instruction * instruction = malloc(sizeof(t_kernel_instruction));
	instruction->parameters = list_create();

	char** splitLine = string_split(line," ");

	instruction->command= kernel_command_from_string(splitLine[0]);
	for (int i=1; i< string_array_size(splitLine);i++){
		list_add(instruction->parameters, splitLine[i]);
	}

	return instruction;
}


void add_instruction(t_pcb* pcb, t_kernel_instruction* instruction){
	list_add(pcb->instructions, instruction);
}

void add_segment(t_pcb* pcb, t_segment_row* segment){
	list_add(pcb->segmentTable, segment);
}

void add_file(t_pcb* pcb, t_open_file_row* openFile){
	list_add(pcb->openFilesTable, openFile);
}

void free_pcb(t_pcb* pcb){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando free_pcb");

	free(pcb->cpuRegisters);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "CPU_registers liberado");

	list_destroy_and_destroy_elements(pcb->openFilesTable, (void*) destroy_open_files_row);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "open_files_table liberada");

	list_destroy_and_destroy_elements(pcb->segmentTable, (void*) destroy_segment_row);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "segment_table liberada");

	list_destroy_and_destroy_elements(pcb->instructions, (void*) destroy_kernel_instruction);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "instructions liberadas");

	free(pcb);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "pcb liberado");
}

void destroy_kernel_instruction(t_kernel_instruction* instruction) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando destroy_kernel_instruction:");

	list_destroy(instruction->parameters);
	free(instruction);
}

void destroy_segment_row(t_segment_row* segmentRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando destroy_segment_row");

	free(segmentRow);
}

void destroy_open_files_row(t_open_file_row* openFileRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando destroy_open_files_row");

	free(openFileRow);
}

kernel_command kernel_command_from_string(char * command) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Ejecutando kernel_command_from_string, parametros : %s",command));

	for (int i = 0; i < COMMAND_ENUM_SIZE; i++) {
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

int get_pid() {
	//Hay un race condition aca donde si se llamara a esta func en simultaneo
	//podria devolver el mismo currentPid, esto deberia hacerse con semaforos una vez
	//tengamos hilos funcionando, al entrar a este metodo debe chequear si el semaforo esta
	//en verde, de ser asi lo pone en rojo guarda el valor a devolver, modifica la variable,
	//habilita el semáforo y devuelve el valor que habia guardado anteriormente
	currentPid++;
	return currentPid-1;
}

void log_pcb(t_pcb* pcb) {
	t_log_level log_level = LOG_LEVEL_INFO;

	write_to_log(LOG_TARGET_INTERNAL, log_level, "Logging PCB:\n");

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("pid: %d", pcb->pid));

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("Program counter: %d \n", pcb->program_counter));

	t_cpu_register* cpu_register = pcb->cpuRegisters;

	write_to_log(LOG_TARGET_INTERNAL, log_level, "CPU Registers:");

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("AX: %s, BX: %s, CX: %s, DX: %s", cpu_register->AX, cpu_register->BX, cpu_register->CX, cpu_register->DX));
	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("EAX: %s, EBX: %s, ECX: %s, EDX: %s", cpu_register->EAX, cpu_register->EBX, cpu_register->ECX, cpu_register->EDX));
	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("RAX: %s, RBX: %s, RCX: %s, RDX: %s \n", cpu_register->RAX, cpu_register->RBX, cpu_register->RCX, cpu_register->RDX));

	//Segment table
	write_to_log(LOG_TARGET_INTERNAL, log_level, "Segment table:");
	list_iterate(pcb->segmentTable, (void*) write_segment_row_to_internal_logs);


	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("next_burst_estimate: %lf", pcb->nextBurstEstimate));

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("time_arrival_ready: %d \n", pcb->timeArrivalReady));

	//open_files_table
	write_to_log(LOG_TARGET_INTERNAL, log_level, "Open files table:");
	list_iterate(pcb->openFilesTable, (void*) write_open_file_row_to_internal_logs);

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("state: %s \n", stateNames[pcb->state]));


	write_to_log(LOG_TARGET_INTERNAL, log_level, "Instrucciones:");
	list_iterate(pcb->instructions, (void*) write_instruction_to_internal_logs);
}

void write_instruction_to_internal_logs(t_kernel_instruction* instruction) {
	write_info_to_all_logs(kernel_command_as_string(instruction->command));
	list_iterate(instruction->parameters, (void*) write_info_to_all_logs);
}

void write_open_file_row_to_internal_logs(t_open_file_row* openFileRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("file: %s", openFileRow->file));

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("pointer: %s \n", openFileRow->pointer));
}


void write_segment_row_to_internal_logs(t_segment_row* segmentRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("id: %d", segmentRow->id));

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("base_direction: %d", segmentRow->baseDirection));

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("segment_size: %d \n", segmentRow->segmentSize));
}

