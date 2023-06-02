#include "console-module.h"

int main(void) {
	t_console_config *consoleEnv = create_console_config(MODULE_NAME);

	init_logger(MODULE_NAME, consoleEnv->LOG_LEVEL);

	log_config(consoleEnv);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/file-utils - fill_buffer_from_file] Aaaaaa"));

	connect_to_kernel(consoleEnv);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/file-utils - fill_buffer_from_file] Bbbbb"));

	send_instructions_to_kernel();

	handle_kernel_response();

	cleanup(get_kernel_connection(), get_logger());

	return EXIT_SUCCESS;
}
