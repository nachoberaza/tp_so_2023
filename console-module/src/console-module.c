#include "console-module.h"

int main(void) {
	t_console_config *CONSOLE_ENV = create_console_config(MODULE_NAME);

	init_logger(MODULE_NAME, CONSOLE_ENV->LOG_LEVEL);

	log_config(CONSOLE_ENV);

	int kernelConnection = connect_to_server(CONSOLE_ENV->IP_KERNEL,
			CONSOLE_ENV->PORT_KERNEL);

	t_package* package = create_package();
	read_console_commands(package);
	send_package(package, kernelConnection);
	cleanup(kernelConnection, get_logger());

	return EXIT_SUCCESS;
}
