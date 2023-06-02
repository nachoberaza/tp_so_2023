#ifndef UTILS_RESOURCE_UTILS_H_
#define UTILS_RESOURCE_UTILS_H_

	#include <stdlib.h>
	#include <string.h>
	#include <commons/collections/list.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include "../structs/resource.h"
	#include "logger-utils.h"
	#include "pcb-utils.h"

	void start_resources_list();
	t_list* get_resources_list();
	t_resource* create_resource(char* name, char* instance);
	void build_resources_list(char** resources, char** instances);
	void log_resource(t_resource* resource);
	t_resource* get_resource(char* resourceName);
	void check_null_resource(t_resource* resource);

#endif
