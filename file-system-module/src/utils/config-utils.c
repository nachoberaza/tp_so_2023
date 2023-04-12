#include "config-utils.h"

t_file_system_config* create_file_system_config(char *moduleName) {
	char *fileName = string_from_format("%s.config", moduleName);

	t_config *config = config_create(fileName);
	t_file_system_config *fileSystemConfig = malloc(sizeof(t_file_system_config));

	fileSystemConfig->IP = config_get_string_value(config, "IP");
	fileSystemConfig->PORT = config_get_string_value(config, "PORT");
	fileSystemConfig->IP_MEMORY = config_get_string_value(config, "IP_MEMORIA");
	fileSystemConfig->PORT_MEMORY = config_get_string_value(config, "PUERTO_MEMORIA");
	fileSystemConfig->PORT_LISTEN = config_get_string_value(config, "PUERTO_ESCUCHA");
	fileSystemConfig->PATH_SUPERBLOQUE = config_get_string_value(config, "PATH_SUPERBLOQUE");
	fileSystemConfig->PATH_BITMAP = config_get_string_value(config, "PATH_BITMAP");
	fileSystemConfig->PATH_BLOQUES = config_get_string_value(config, "PATH_BLOQUES");
	fileSystemConfig->PATH_FCB = config_get_string_value(config, "PATH_FCB");
	fileSystemConfig->BLOCK_ACCESS_DELAY = config_get_int_value(config, "RETARDO_ACCESO_BLOQUE");
	fileSystemConfig->LOG_LEVEL = log_level_from_string(config_get_string_value(config, "LOG_LEVEL"));

	return fileSystemConfig;
}

void log_config(t_file_system_config *config) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "FILE SYSTEM CONFIG VALUES: ");
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("IP: %s", config->IP));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT: %s", config->PORT));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("IP_MEMORY: %s", config->IP_MEMORY));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT_MEMORY: %s", config->PORT_MEMORY));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT_LISTEN: %s", config->PORT_LISTEN));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_SUPERBLOQUE: %s", config->PATH_SUPERBLOQUE));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_BITMAP: %s", config->PATH_BITMAP));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_BLOQUES: %s", config->PATH_BLOQUES));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_FCB: %s", config->PATH_FCB));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("BLOCK_ACCESS_DELAY: %s", string_itoa(config->BLOCK_ACCESS_DELAY)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("FILE_SYSTEM_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}
