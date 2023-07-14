#include "config-utils.h"

t_memory_config *memoryConfig;

void create_memory_config(char *fileName) {
	t_config *config = config_create(fileName);
	memoryConfig = malloc(sizeof(t_memory_config));

	memoryConfig->IP = config_get_string_value(config, "IP");
	memoryConfig->PORT = config_get_string_value(config, "PUERTO_ESCUCHA");
	memoryConfig->MEMORY_SIZE = config_get_int_value(config, "TAM_MEMORIA");
	memoryConfig->SEGMENT_ZERO_SIZE = config_get_int_value(config, "TAM_SEGMENTO_0");
	memoryConfig->SEGMENT_COUNT = config_get_int_value(config, "CANT_SEGMENTOS");
	memoryConfig->MEMORY_DELAY = config_get_int_value(config, "RETARDO_MEMORIA");
	memoryConfig->COMPACTION_DELAY = config_get_int_value(config, "RETARDO_COMPACTACION");
	memoryConfig->ASSIGNMENT_ALGORITHM = config_get_string_value(config, "ALGORITMO_ASIGNACION");
	memoryConfig->LOG_LEVEL = log_level_from_string(config_get_string_value(config, "LOG_LEVEL"));
}

t_memory_config* get_memory_config() {
	return memoryConfig;
}

t_assignment_algorithm get_assignment_algorithm() {
	if(strcmp(memoryConfig->ASSIGNMENT_ALGORITHM, "FIRST") == 0){
		return FIRST;
	} else if(strcmp(memoryConfig->ASSIGNMENT_ALGORITHM, "BEST") == 0) {
		return BEST;
	}

	return WORST;
}

void log_config(t_memory_config *config) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "MEMORY CONFIG VALUES: ");
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP: %s", config->IP));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT: %s", config->PORT));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("MEMORY_SIZE: %s", string_itoa(config->MEMORY_SIZE)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("INITIAL_MEMORY_SEGMENT: %s", string_itoa(config->SEGMENT_ZERO_SIZE)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("SEGMENT_COUNT: %s", string_itoa(config->SEGMENT_COUNT)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("MEMORY_DELAY: %s", string_itoa(config->MEMORY_DELAY)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("COMPACTION_DELAY: %s", string_itoa(config->COMPACTION_DELAY)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("ASSIGNMENT_ALGORITHM: %s", config->ASSIGNMENT_ALGORITHM));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("KERNEL_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}
