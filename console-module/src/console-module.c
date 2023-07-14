#include "console-module.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

	t_console_config *consoleEnv = create_console_config(argv[1]);

	init_logger(MODULE_NAME, consoleEnv->LOG_LEVEL);

	log_config(consoleEnv);

	connect_to_kernel(consoleEnv);

	send_instructions_to_kernel(argv[2]);

	handle_kernel_response();

	cleanup(get_kernel_connection(), get_logger());

	return EXIT_SUCCESS;
}
