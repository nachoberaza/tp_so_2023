#include "console-module.h"

int main(void) {
	t_console_config *consoleEnv = create_console_config(MODULE_NAME);

	init_logger(MODULE_NAME, consoleEnv->LOG_LEVEL);

	log_config(consoleEnv);

	connect_to_kernel(consoleEnv);

	send_instructions_to_kernel();

	handle_kernel_response();

	cleanup(get_kernel_connection(), get_logger());

	return EXIT_SUCCESS;
}
