#include "pcb-utils.h"

int currentPid = 0;
t_list * pcbList;

void start_pcb_list() {
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
	pcb->executionContext->pid = get_current_pid();
	pcb->executionContext->cpuRegisters = malloc(sizeof(t_cpu_register));
	pcb->executionContext->instructions = list_create();
	pcb->executionContext->programCounter = 0;
	pcb->state = NEW;

	return pcb;
}

// TODO: Eliminar valores hardcodeados.
void build_pcb(t_list *lines, int clientSocketId) {
	t_pcb* pcb = create_pcb_from_lines(lines, clientSocketId);

	t_open_file_row* openFileRow = malloc(sizeof(t_open_file_row));

	openFileRow->file = "asd";
	openFileRow->pointer = "asd2";
	add_file(pcb, openFileRow);

	t_segment_row* segmentRow = malloc(sizeof(t_segment_row));
	add_segment(pcb, segmentRow);

	pcb->executionContext->exitReason = REASON_BLOCK;

	strncpy(pcb->executionContext->cpuRegisters->AX, "Hi", sizeof(char) * 4);
	t_instruction* firstInstruction = list_get(pcb->executionContext->instructions, 0);

	char* valueParameter = list_get(firstInstruction->parameters, 1);
	strncpy(pcb->executionContext->cpuRegisters->AX, valueParameter, sizeof(char) * 4);

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

void populate_instruction_list_from_lines(t_list* instructions, t_list* lines){
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
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_DEBUG,
			string_from_format("[utils/pcb-utils - free_pcb] Liberando de memoria PID: %d", pcb->executionContext->pid)
	);

	destroy_execution_context(get_logger(), pcb->executionContext);
	list_destroy_and_destroy_elements(pcb->openFilesTable, (void*) destroy_open_files_row);
	list_destroy_and_destroy_elements(pcb->segmentTable, (void*) destroy_segment_row);
	free(pcb);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/pcb-utils - free_pcb] PCB Liberado");
}


void destroy_segment_row(t_segment_row* segmentRow) {
	free(segmentRow);
}

void destroy_open_files_row(t_open_file_row* openFileRow) {
	free(openFileRow);
}

int get_current_pid() {
	//Hay un race condition aca donde si se llamara a esta func en simultaneo
	//podria devolver el mismo currentPid, esto deberia hacerse con semaforos una vez
	//tengamos hilos funcionando, al entrar a este metodo debe chequear si el semaforo esta
	//en verde, de ser asi lo pone en rojo guarda el valor a devolver, modifica la variable,
	//habilita el semáforo y devuelve el valor que habia guardado anteriormente
	currentPid++;
	return currentPid-1;
}

void log_pcb(t_pcb* pcb) {
	t_log_level logLevel = LOG_LEVEL_DEBUG;

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/pcb-utils - log_pcb] Logeando PCB:\n");

	log_context(get_logger(), LOG_LEVEL_INFO, pcb->executionContext);
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("ClientSocketId: %d", pcb->clientSocketId));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("NextBurstEstimate: %lf", pcb->nextBurstEstimate));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("TimeArrivalReady: %d \n", pcb->timeArrivalReady));

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/pcb-utils - log_pcb] Segment table:");
	list_iterate(pcb->segmentTable, (void*) write_segment_row_to_internal_logs);

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/pcb-utils - log_pcb] Open files table:");
	list_iterate(pcb->openFilesTable, (void*) write_open_file_row_to_internal_logs);

	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("[utils/pcb-utils - log_pcb] State: %s \n", stateNames[pcb->state]));
}


void write_open_file_row_to_internal_logs(t_open_file_row* openFileRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("File: %s", openFileRow->file));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Pointer: %s \n", openFileRow->pointer));
}


void write_segment_row_to_internal_logs(t_segment_row* segmentRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Id: %d", segmentRow->id));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("BaseDirection: %d", segmentRow->baseDirection));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("SegmentSize: %d \n", segmentRow->segmentSize));
}

