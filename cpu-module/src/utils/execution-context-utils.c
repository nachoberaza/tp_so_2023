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
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/execution-context-utils - execute_execution_context] Ejecutando proceso: %d", context->pid));

	while (context->programCounter < size){
		int result;
		t_instruction* current = list_get(context->instructions, context->programCounter);
		switch(current->command){
			case SET:
				result = execute_set(context);
				break;
			case YIELD:
				result = execute_yield(context);
				break;
			case EXIT:
				result = execute_exit(context);
				break;
			case WAIT:
				result = execute_wait(context);
				break;
			case SIGNAL:
				result = execute_signal(context);
				break;
			case IO:
				result = execute_io(context);
				break;
			case MOV_IN:
					// TODO: result = execute_mov_in(context);
					break;
			case MOV_OUT:
					// TODO: result = execute_mov_out(context);
					break;
			case F_OPEN:
					// TODO: result = execute_f_open(context);
					break;
			case F_CLOSE:
					// TODO: result = execute_f_close(context);
					break;
			case F_SEEK:
					// TODO: result = execute_f_seek(context);
					break;
			case F_READ:
					// TODO: result = execute_f_read(context);
					break;
			case F_WRITE:
					// TODO: result = execute_f_write(context);
					break;
			case F_TRUNCATE:
					// TODO: result = execute_f_truncate(context);
					break;
			case CREATE_SEGMENT:
					// TODO: result = execute_create_segment(context);
					break;
			case DELETE_SEGMENT:
					// TODO: result = execute_delete_segment(context);
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

	context->reason->executionContextState = REASON_FINISH;
}
