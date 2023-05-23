#include "execution-context-utils.h"

void return_execution_context(t_execution_context* context, int clientSocketId) {

	t_package* serializedPackage = create_package();

	fill_package_with_context(get_logger(), context, serializedPackage);

	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			string_from_format("[utils/execution-context-utils - return_execution_context] Devuelvo contexto actualizado del PID: %d a kernel", context->pid)
	);
	send_package(serializedPackage, clientSocketId);

	destroy_execution_context(get_logger(), context);
}

void execute_execution_context(t_execution_context* context) {
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
			write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/execution-context-utils - execute_execution_context] Comando no pertenece al CPU");
			return;
			break;
		}
		context->programCounter++;
		if (!result)
			return;
	}

	context->exitReason = REASON_FINISH;
}
