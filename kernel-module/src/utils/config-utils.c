#include "config-utils.h"

t_kernel_config* create_kernel_config(char *moduleName) {
	char *fileName = string_from_format("%s.config", moduleName);

	t_config *config = config_create(fileName);
	t_kernel_config *kernelConfig = malloc(sizeof(t_kernel_config));

	kernelConfig->IP = config_get_string_value(config, "IP");
	kernelConfig->PORT = config_get_string_value(config, "PUERTO_ESCUCHA");
	kernelConfig->IP_MEMORY = config_get_string_value(config, "IP_MEMORIA");
	kernelConfig->PORT_MEMORY = config_get_string_value(config, "PUERTO_MEMORIA");
	kernelConfig->IP_FILESYSTEM = config_get_string_value(config, "IP_FILESYSTEM");
	kernelConfig->PORT_FILESYSTEM = config_get_string_value(config, "PUERTO_FILESYSTEM");
	kernelConfig->IP_CPU = config_get_string_value(config, "IP_CPU");
	kernelConfig->PORT_CPU = config_get_string_value(config, "PUERTO_CPU");
	kernelConfig->PLANNING_ALGORITHM = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	kernelConfig->INITIAL_ESTIMATE = config_get_int_value(config, "ESTIMACION_INICIAL");
	kernelConfig->HRRN_ALFA = config_get_int_value(config, "HRRN_ALFA");
	kernelConfig->RESOURCES = config_get_array_value(config, "RECURSOS");
	kernelConfig->RESOURCES_INSTANCES = config_get_array_value(config, "INSTANCIAS_RECURSOS");
	kernelConfig->MAX_MULTIPROGRAMMING_LEVEL = config_get_int_value(config, "GRADO_MAX_MULTIPROGRAMACION");
	kernelConfig->LOG_LEVEL = log_level_from_string(config_get_string_value(config, "LOG_LEVEL"));

	return kernelConfig;
}

void log_config(t_kernel_config *config) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "KERNEL CONFIG VALUES: ");
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP: %s", config->IP));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT: %s", config->PORT));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP_MEMORY: %s", config->IP_MEMORY));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT_MEMORY: %s", config->PORT_MEMORY));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP_FILESYSTEM: %s", config->IP_FILESYSTEM));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT_FILESYSTEM: %s", config->PORT_FILESYSTEM));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("IP_CPU: %s", config->IP_CPU));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PORT_CPU: %s", config->PORT_CPU));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("PLANNING_ALGORITHM: %s", config->PLANNING_ALGORITHM));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("INITIAL_ESTIMATE: %s", string_itoa(config->INITIAL_ESTIMATE)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("HRRN_ALFA: %s", string_itoa(config->HRRN_ALFA)));

	//contemplamos que ambas listas tiene el = size
	for (int i = 0; i < string_array_size(config->RESOURCES); i++){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("RESOURCES %d: %s", i, config->RESOURCES[i]));
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("INSTANCES %d: %s", i, config->RESOURCES_INSTANCES[i]));
	}

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("RESOURCES_INSTANCES: %s", string_itoa(config->HRRN_ALFA)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("MAX_MULTIPROGRAMMING_LEVEL: %s", string_itoa(config->MAX_MULTIPROGRAMMING_LEVEL)));
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("KERNEL_LOG_LEVEL: %s", log_level_as_string(config->LOG_LEVEL)));
}
