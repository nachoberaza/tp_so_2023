#include "pcb-utils.h"

int currentPid = 0;
t_list * newPcbList;
t_list * shortTermPcbList;

void start_pcb_list() {
	newPcbList = list_create();
	shortTermPcbList = list_create();
}

t_list* get_new_pcb_list(){
	return newPcbList;
}

t_list* get_short_term_list(){
	return shortTermPcbList;
}

t_pcb* new_pcb(int clientSocketId){
	t_pcb *pcb = malloc(sizeof(t_pcb));

	pcb->state = STATE_NEW;
	pcb->clientSocketId = clientSocketId;
	pcb->nextBurstEstimate = get_kernel_config()->INITIAL_ESTIMATE;
	pcb->timeArrivalReady = (int)time(NULL);
	pcb->segmentTable = list_create();
	pcb->openFilesTable = list_create();
	pcb->executionContext = init_execution_context(get_current_pid());
	pcb->firstTimeInReady = true;

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

	pcb->executionContext->reason->executionContextState = REASON_YIELD;

	strncpy(pcb->executionContext->cpuRegisters->AX, "Hi", sizeof(char) * 4);
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

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format("[utils/pcb-utils - build_pcb] Se crea el proceso %d en NEW", pcb->executionContext->pid)
	);

	list_add(newPcbList, pcb);
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
			LOG_LEVEL_TRACE,
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
	t_log_level logLevel = LOG_LEVEL_TRACE;

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/pcb-utils - log_pcb] Logeando PCB:\n");

	log_context(get_logger(), LOG_LEVEL_TRACE, pcb->executionContext);
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("ClientSocketId: %d", pcb->clientSocketId));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("NextBurstEstimate: %lf", pcb->nextBurstEstimate));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("TimeArrivalReady: %d \n", pcb->timeArrivalReady));

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/pcb-utils - log_pcb] Segment table:");
	list_iterate(pcb->segmentTable, (void*) write_segment_row_to_internal_logs);

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/pcb-utils - log_pcb] Open files table:");
	list_iterate(pcb->openFilesTable, (void*) write_open_file_row_to_internal_logs);

	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("[utils/pcb-utils - log_pcb] State: %s \n", state_as_string(pcb->state)));
}


void write_open_file_row_to_internal_logs(t_open_file_row* openFileRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("File: %s", openFileRow->file));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("Pointer: %s \n", openFileRow->pointer));
}


void write_segment_row_to_internal_logs(t_segment_row* segmentRow) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("Id: %d", segmentRow->id));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("BaseDirection: %d", segmentRow->baseDirection));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("SegmentSize: %d \n", segmentRow->segmentSize));
}

void log_pcb_state(t_pcb* pcb){
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_DEBUG,
			string_from_format("[utils/pcb-utils - log_pcb_state] PID: %d, estado: %s", pcb->executionContext->pid, state_as_string(pcb->state))
	);
}

int pcb_is_ready(t_pcb* pcb){
	return (pcb->state == STATE_READY);
}

int pcb_is_running(t_pcb* pcb){
	return (pcb->state == STATE_RUNNING);
}

int pcb_is_not_blocked(t_pcb* pcb){
	return (pcb->state != STATE_BLOCK);
}

void move_to_ready(t_pcb* pcb){

	write_to_log(
		LOG_TARGET_MAIN,
		LOG_LEVEL_INFO,
		string_from_format(
				"Cambio de Estado: “PID: %d - Estado Anterior: %s - Estado Actual: %s",
				pcb->executionContext->pid,state_as_string(pcb->state),state_as_string(STATE_READY))
	);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		string_from_format("[utils/pcb-utils - move_to_ready] Moviendo PID: %d", pcb->executionContext->pid)
	);

	pcb->state = STATE_READY;
}

void move_to_running(t_pcb* pcb){

	write_to_log(
			LOG_TARGET_MAIN,
			LOG_LEVEL_INFO,
			string_from_format(
					"Cambio de Estado: “PID: %d - Estado Anterior: %s - Estado Actual: %s",
					pcb->executionContext->pid,state_as_string(pcb->state),state_as_string(STATE_RUNNING))
	);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_TRACE,
		string_from_format("[utils/pcb-utils - move_to_running] Moviendo PID: %d", pcb->executionContext->pid)
	);

	pcb->state = STATE_RUNNING;
}

void move_to_blocked(t_pcb* pcb){

	write_to_log(
				LOG_TARGET_MAIN,
				LOG_LEVEL_INFO,
				string_from_format(
						"Cambio de Estado: “PID: %d - Estado Anterior: %s - Estado Actual: %s",
						pcb->executionContext->pid,state_as_string(pcb->state),state_as_string(STATE_BLOCK))
	);

	write_to_log(
						LOG_TARGET_INTERNAL,
						LOG_LEVEL_TRACE,
						string_from_format("[utils/pcb-utils - move_to_blocked] Moviendo PID: %d", pcb->executionContext->pid)
				);

	pcb->state = STATE_BLOCK;
}

char * state_as_string(state_process state) {
	return stateNames[state];
}

char* get_string_array_pid(t_list* list){
	char* pids= string_new();
	int sizeList= list_size(list);

	for(int i=0; i < sizeList;i++){
		t_pcb* pcb = list_get(list,i);
		string_append_with_format(&pids, ((i == sizeList-1) ? "%s":"%s,") , string_itoa(pcb->executionContext->pid));
	}

	return pids;
}

