#include "pcb-utils.h"

int currentPid = 0;
t_list * pcbList;

void start_pcb_list() {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "PCB List creado");
	pcbList = list_create();
}

t_list* get_pcb_list(){
	return pcbList;
}

t_pcb* new_pcb(int clientSocketId){
	t_pcb *pcb = malloc(sizeof(t_pcb));

	pcb->clientSocketId = clientSocketId;
	pcb->segmentTable = list_create();
	pcb->openFilesTable = list_create();
	pcb->executionContext = malloc(sizeof(t_execution_context));
	pcb->executionContext->pid = get_pid();
	pcb->executionContext->cpuRegisters = malloc(sizeof(t_cpu_register));
	pcb->executionContext->instructions = list_create();
	pcb->executionContext->programCounter = 0;
	pcb->state = NEW;

	return pcb;
}

void build_pcb(t_list *lines, int clientSocketId) {
	t_pcb* pcb = create_pcb_from_lines(lines, clientSocketId);

	t_open_file_row* open_file_row = malloc(sizeof(t_open_file_row));
	// TODO: Eliminar valores hardcodeados.
	open_file_row->file = "asd";
	open_file_row->pointer = "asd2";
	add_file(pcb, open_file_row);

	t_segment_row* segment_row = malloc(sizeof(t_segment_row));
	add_segment(pcb, segment_row);

	pcb->executionContext->exitReason = REASON_BLOCK;

	strncpy(pcb->executionContext->cpuRegisters->AX, "Hi", sizeof(char) * 4);
	t_instruction* inst = list_get(pcb->executionContext->instructions, 0);

	char* value = list_get(inst->parameters, 1);
	strncpy(pcb->executionContext->cpuRegisters->AX, value, sizeof(char) * 4);

	strncpy(pcb->executionContext->cpuRegisters->BX, "Hi", sizeof(char) * 4);
	strncpy(pcb->executionContext->cpuRegisters->CX, "Hi", sizeof(char) * 4);
	strncpy(pcb->executionContext->cpuRegisters->DX, "Hi", sizeof(char) * 4);

	strncpy(pcb->executionContext->cpuRegisters->EAX, "Hello", sizeof(char) * 8);
	strncpy(pcb->executionContext->cpuRegisters->EBX, "Hello", sizeof(char) * 8);
	strncpy(pcb->executionContext->cpuRegisters->ECX, "Hello", sizeof(char) * 8);
	strncpy(pcb->executionContext->cpuRegisters->EDX, "Hello", sizeof(char) * 8);

	strncpy(pcb->executionContext->cpuRegisters->RAX, "Hello there", sizeof(char) * 16);
	strncpy(pcb->executionContext->cpuRegisters->RBX, "Hello there", sizeof(char) * 16);
	strncpy(pcb->executionContext->cpuRegisters->RCX, "Hello there", sizeof(char) * 16);
	strncpy(pcb->executionContext->cpuRegisters->RDX, "Hello there", sizeof(char) * 16);


	log_pcb(pcb);

	list_add(pcbList, pcb);
}

t_pcb* create_pcb_from_lines(t_list* lines, int clientSocketId){
	t_pcb *pcb = new_pcb(clientSocketId);

	populate_instruction_list_from_lines(pcb->executionContext->instructions,lines);

	return pcb;
}

void populate_instruction_list_from_lines(t_list* instructions,t_list* lines){
	for (int i=0; i < list_size(lines); i++){
		t_instruction * instruction = create_instruction(list_get(lines, i));
		list_add(instructions, instruction);
	}
}

t_instruction* create_instruction(char* line){
	t_instruction * instruction = malloc(sizeof(t_instruction));
	instruction->parameters = list_create();

	char** splitLine = string_split(line," ");

	instruction->command= command_from_string(get_logger(), splitLine[0]);
	for (int i=1; i< string_array_size(splitLine);i++){
		list_add(instruction->parameters, splitLine[i]);
	}

	return instruction;
}


void add_instruction(t_pcb* pcb, t_instruction* instruction){
	list_add(pcb->executionContext->instructions, instruction);
}

void add_segment(t_pcb* pcb, t_segment_row* segment){
	list_add(pcb->segmentTable, segment);
}

void add_file(t_pcb* pcb, t_open_file_row* openFile){
	list_add(pcb->openFilesTable, openFile);
}

void free_pcb(t_pcb* pcb){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando free_pcb");

	destroy_execution_context(get_logger(), pcb->executionContext);

	list_destroy_and_destroy_elements(pcb->openFilesTable, (void*) destroy_open_files_row);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "open_files_table liberada");

	list_destroy_and_destroy_elements(pcb->segmentTable, (void*) destroy_segment_row);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "segment_table liberada");


	free(pcb);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "pcb liberado");
}


void destroy_segment_row(t_segment_row* segmentRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando destroy_segment_row");

	free(segmentRow);
}

void destroy_open_files_row(t_open_file_row* openFileRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "Ejecutando destroy_open_files_row");

	free(openFileRow);
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

	log_context(get_logger(), LOG_LEVEL_INFO, pcb->executionContext);
	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("clientSocketId: %d", pcb->clientSocketId));

	//Segment table
	write_to_log(LOG_TARGET_INTERNAL, log_level, "Segment table:");
	list_iterate(pcb->segmentTable, (void*) write_segment_row_to_internal_logs);


	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("next_burst_estimate: %lf", pcb->nextBurstEstimate));

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("time_arrival_ready: %d \n", pcb->timeArrivalReady));

	//open_files_table
	write_to_log(LOG_TARGET_INTERNAL, log_level, "Open files table:");
	list_iterate(pcb->openFilesTable, (void*) write_open_file_row_to_internal_logs);

	write_to_log(LOG_TARGET_INTERNAL, log_level, string_from_format("state: %s \n", stateNames[pcb->state]));

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

