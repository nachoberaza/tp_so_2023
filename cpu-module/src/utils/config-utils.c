#include "config-utils.h"

t_cpu_config * cpuEnv;

t_cpu_config* get_cpu_env(){
	return cpuEnv;
}

void create_cpu_config(char *fileName) {
	t_config *config = config_create(fileName);
	cpuEnv = malloc(sizeof(t_cpu_config));

	cpuEnv->IP = config_get_string_value(config, "IP");
	cpuEnv->INSTRUCTION_DELAY = config_get_int_value(config, "RETARDO_INSTRUCCION");
	cpuEnv->IP_MEMORY = config_get_string_value(config, "IP_MEMORIA");
	cpuEnv->PORT_MEMORY = config_get_string_value(config, "PUERTO_MEMORIA");
	cpuEnv->PORT = config_get_string_value(config, "PUERTO_ESCUCHA");
	cpuEnv->SEGMENT_MAX_SIZE = config_get_int_value(config, "TAM_MAX_SEGMENTO");
	cpuEnv->LOG_LEVEL = log_level_from_string(config_get_string_value(config, "LOG_LEVEL"));

}

void log_config(t_cpu_config *config) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "CPU CONFIG VALUES: ");
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP: %s", config->IP));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT: %s", config->PORT));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP_MEMORY: %s", config->IP_MEMORY));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT_MEMORY: %s", config->PORT_MEMORY));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("INSTRUCTION_DELAY: %s", string_itoa(config->INSTRUCTION_DELAY)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("SEGMENT_MAX_SIZE: %s", string_itoa(config->SEGMENT_MAX_SIZE)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("CPU_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}
