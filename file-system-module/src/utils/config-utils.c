#include "config-utils.h"

t_file_system_config  *fileSystemConfig;
t_super_block_config *superBlockConfig;

void init_file_system_config(char *fileName) {
	t_config *config = config_create(fileName);
	fileSystemConfig = malloc(sizeof(t_file_system_config));

	fileSystemConfig->IP = config_get_string_value(config, "IP");
	fileSystemConfig->IP_MEMORY = config_get_string_value(config, "IP_MEMORIA");
	fileSystemConfig->PORT_MEMORY = config_get_string_value(config, "PUERTO_MEMORIA");
	fileSystemConfig->PORT = config_get_string_value(config, "PUERTO_ESCUCHA");
	fileSystemConfig->PATH_SUPERBLOQUE = config_get_string_value(config, "PATH_SUPERBLOQUE");
	fileSystemConfig->PATH_BITMAP = config_get_string_value(config, "PATH_BITMAP");
	fileSystemConfig->PATH_BLOQUES = config_get_string_value(config, "PATH_BLOQUES");
	fileSystemConfig->PATH_FCB = config_get_string_value(config, "PATH_FCB");
	fileSystemConfig->BLOCK_ACCESS_DELAY = config_get_int_value(config, "RETARDO_ACCESO_BLOQUE");
	fileSystemConfig->LOG_LEVEL = log_level_from_string(config_get_string_value(config, "LOG_LEVEL"));

}

t_file_system_config* get_file_system_config() {
	return fileSystemConfig;
}

t_super_block_config* get_super_block_config() {
	return superBlockConfig;
}

void log_config(t_file_system_config *config) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "FILE SYSTEM CONFIG VALUES: ");
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("IP: %s", config->IP));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("IP_MEMORY: %s", config->IP_MEMORY));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT_MEMORY: %s", config->PORT_MEMORY));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT_LISTEN: %s", config->PORT));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_SUPERBLOQUE: %s", config->PATH_SUPERBLOQUE));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_BITMAP: %s", config->PATH_BITMAP));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_BLOQUES: %s", config->PATH_BLOQUES));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PATH_FCB: %s", config->PATH_FCB));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("BLOCK_ACCESS_DELAY: %s", string_itoa(config->BLOCK_ACCESS_DELAY)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("FILE_SYSTEM_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}

void load_super_block() {
	char *filePath = get_file_system_config()->PATH_SUPERBLOQUE;
	t_config *config = config_create(filePath);
	superBlockConfig = malloc(sizeof(t_super_block_config));

	superBlockConfig->BLOCK_SIZE = config_get_int_value(config, "BLOCK_SIZE");
	superBlockConfig->BLOCK_COUNT = config_get_int_value(config, "BLOCK_COUNT");
	log_super_block(get_super_block_config());
}

void log_super_block(t_super_block_config *superBlockConfig) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "SUPERBLOQUE VALUES: ");
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("BLOCK_SIZE: %s", string_itoa(superBlockConfig->BLOCK_SIZE)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("BLOCK_COUNT: %s", string_itoa(superBlockConfig->BLOCK_COUNT)));
}
