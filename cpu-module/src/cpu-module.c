#include "cpu-module.h"


int main(void) {
	t_cpu_config *CPU_ENV = create_cpu_config(MODULE_NAME);
	init_logger(MODULE_NAME, CPU_ENV->LOG_LEVEL);

	log_config(CPU_ENV);

	int connectionMemory = connect_to_server(CPU_ENV->IP_MEMORY, CPU_ENV->PORT_MEMORY);
	handle_memory_handshake(connectionMemory, CPU);

	int serverSocketId = start_server(CPU_ENV->IP, CPU_ENV->PORT, get_logger());

	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "Servidor listo para recibir al cliente");

	int clientSocketId = await_client(get_logger(), serverSocketId);

	t_execution_context *context;
	//Matar variables post serializado
	//Usar buffer en vez de package
	//Eso tiene q estar en un utils




	while (1) {
		receive_operation_code(clientSocketId);
		context = decode_context(get_logger(), LOG_LEVEL_DEBUG, clientSocketId);
		log_context(get_logger(), LOG_LEVEL_DEBUG, context);
		execute_context(context);
		return_context(context, clientSocketId);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Sleeping 5");
		sleep(5);
	}

	return EXIT_SUCCESS;
}

void return_context(t_execution_context* context, int clientSocketId) {

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Before sending");
	t_package* serializedPackage = create_package();
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Package created");

	fill_package_with_context(get_logger(),context,serializedPackage);

	send_package(serializedPackage, clientSocketId);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Send package");

	destroy_execution_context(get_logger(), context);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, "Destroying package");
}

void execute_context(t_execution_context* context) {
	int size = list_size(context->instructions);
	while (context->programCounter <= size){
		int result;
		t_instruction* current = list_get(context->instructions, context->programCounter);
		switch(current->command){
		case SET:
			result = execute_set(context,current);
			break;
		case YIELD:
			result = execute_yield(context);
			break;
		case EXIT:
			result = execute_exit(context);
			break;
		default:
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Comando no pertenece al CPU");
			return;
			break;
		}
		context->programCounter++;
		if (!result)
			return;
	}

	context->exitReason = REASON_FINISH;
}

int execute_set(t_execution_context* context, t_instruction* currentInstruction){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Ejecutando SET");

	char* reg = list_get(currentInstruction->parameters,0);
	char* value = list_get(currentInstruction->parameters,1);

	//TODO:mejorar if feos
	if(strcmp(reg,"AX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register AX");
		strncpy(context->cpuRegisters->AX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"BX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register BX");
		strncpy(context->cpuRegisters->BX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"CX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register CX");
		strncpy(context->cpuRegisters->CX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"DX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register DX");
		strncpy(context->cpuRegisters->DX, value, sizeof(char)*4);
		return 1;
	}

	if(strcmp(reg,"EAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register EAX");
		strncpy(context->cpuRegisters->EAX, value, sizeof(char)*8);
		return 1;

	}

	if(strcmp(reg,"EBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register EBX");
		strncpy(context->cpuRegisters->EBX, value, sizeof(char)*8);
		return 1;
	}

	if(strcmp(reg,"ECX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register ECX");
		strncpy(context->cpuRegisters->ECX, value, sizeof(char)*8);
		return 1;
	}

	if(strcmp(reg,"EDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register EDX");
		strncpy(context->cpuRegisters->EDX, value, sizeof(char)*8);
		return 1;
	}

	if(strcmp(reg,"RAX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register RAX");
		strncpy(context->cpuRegisters->RAX, value, sizeof(char)*16);
		return 1;
	}

	if(strcmp(reg,"RBX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register RBX");
		strncpy(context->cpuRegisters->RBX, value, sizeof(char)*16);
		return 1;
	}

	if(strcmp(reg,"RCX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register RCX");
		strncpy(context->cpuRegisters->RCX, value, sizeof(char)*16);
		return 1;
	}

	if(strcmp(reg,"RDX") == 0){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Asignando Register RDX");
		strncpy(context->cpuRegisters->RDX, value, sizeof(char)*16);
		return 1;
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "Registro no valido");

	return 0;
}

int execute_yield(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Ejecutando YIELD");
	context->exitReason = REASON_BLOCK;
	return 0;
}

int execute_exit(t_execution_context* context){
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Ejecutando EXIT");
	context->exitReason = REASON_FINISH;
	return 0;
}




