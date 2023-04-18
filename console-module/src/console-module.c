#include "console-module.h"

int main(void) {
	int connection;

	t_console_config *CONSOLE_ENV = create_console_config(MODULE_NAME);

	init_logger(MODULE_NAME, CONSOLE_ENV->LOG_LEVEL);

	log_config(CONSOLE_ENV);

	connection = connect_to_server(CONSOLE_ENV->IP_KERNEL,
			CONSOLE_ENV->PORT_KERNEL);

	build_package(connection);

	cleanup(connection, get_logger());

	return EXIT_SUCCESS;
}
