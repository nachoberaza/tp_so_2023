#include "config-utils.h"

t_console_config* create_console_config(char *moduleName) {
	char *fileName = string_from_format("%s.config", moduleName);

	t_config *config = config_create(fileName);
	t_console_config *consoleConfig = malloc(sizeof(t_console_config));

	consoleConfig->IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
	consoleConfig->PORT_KERNEL = config_get_string_value(config,
			"PUERTO_KERNEL");

	consoleConfig->LOG_LEVEL = log_level_from_string(
			config_get_string_value(config, "LOG_LEVEL"));

	return consoleConfig;
}

void log_config(t_console_config *config) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "CONSOLE CONFIG VALUES: ");
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("IP_KERNEL: %s", config->IP_KERNEL));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT_KERNEL: %s", config->PORT_KERNEL));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("CONSOLE_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}
