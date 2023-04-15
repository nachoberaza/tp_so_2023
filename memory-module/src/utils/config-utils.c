#include "config-utils.h"

t_memory_config* create_memory_config(char *moduleName) {
	char *fileName = string_from_format("%s.config", moduleName);

	t_config *config = config_create(fileName);
	t_memory_config *memoryConfig = malloc(sizeof(t_memory_config));

	memoryConfig->IP = config_get_string_value(config, "IP");
	memoryConfig->PORT = config_get_string_value(config, "PUERTO_ESCUCHA");
	memoryConfig->MEMORY_SIZE = config_get_int_value(config, "TAM_MEMORIA");
	memoryConfig->SHARED_MEMORY_SEGMENT = config_get_int_value(config, "TAM_SEGMENTO_0");
	memoryConfig->SEGMENT_COUNT = config_get_int_value(config, "CANT_SEGMENTOS");
	memoryConfig->MEMORY_DELAY = config_get_int_value(config, "RETARDO_MEMORIA");
	memoryConfig->COMPACTION_DELAY = config_get_int_value(config, "RETARDO_COMPACTACION");
	memoryConfig->ASSIGNMENT_ALGORITHM = config_get_string_value(config, "ALGORITMO_ASIGNACION");
	memoryConfig->LOG_LEVEL = log_level_from_string(config_get_string_value(config, "LOG_LEVEL"));

	return memoryConfig;
}

void log_config(t_memory_config *config) {
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, "MEMORY CONFIG VALUES: ");
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("IP: %s", config->IP));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("PORT: %s", config->PORT));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("MEMORY_SIZE: %s", string_itoa(config->MEMORY_SIZE)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("INITIAL_MEMORY_SEGMENT: %s", string_itoa(config->SHARED_MEMORY_SEGMENT)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("SEGMENT_COUNT: %s", string_itoa(config->SEGMENT_COUNT)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("MEMORY_DELAY: %s", string_itoa(config->MEMORY_DELAY)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("COMPACTION_DELAY: %s", string_itoa(config->COMPACTION_DELAY)));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("ASSIGNMENT_ALGORITHM: %s", config->ASSIGNMENT_ALGORITHM));
	write_to_log(LOG_TARGET_ALL, LOG_LEVEL_INFO, string_from_format("KERNEL_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}