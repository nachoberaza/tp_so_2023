#include "resource-utils.h"

t_list * resourcesList;

void start_resources_list() {
	resourcesList = list_create();
}

t_list* get_resources_list(){
	return resourcesList;
}

t_resource* create_resource(char* name, char* instance){
	t_resource* resource = malloc(sizeof(t_resource));

	resource->blocked = list_create();
	resource->name= name;
	resource->instances= atoi(instance);

	return resource;
}

void build_resources_list(char** resources, char** instances){
	start_resources_list();

	for (int i = 0; i < string_array_size(resources); i++){
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, "[utils/resource-utils - build_resources_list] Create Resource:\n");
		t_resource* resource = create_resource(resources[i],instances[i]);
		log_resource(resource);
		list_add(resourcesList, create_resource(resources[i],instances[i]));
	}
}

void log_resource(t_resource* resource) {
	t_log_level logLevel = LOG_LEVEL_DEBUG;

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/resource-utils - log_resource] Logging Resource:\n");

	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("Name: %s", resource->name));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("Instances: %d", resource->instances));

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "Blocked:");
	list_iterate(resource->blocked, (void*) log_pcb);

}

t_resource* get_resource(char* resourceName){
	int size = list_size(resourcesList);

	for(int i=0; i< size;i++){
		t_resource* resource = list_get(resourcesList,i);

		if(!strcmp(resource->name,resourceName)){
			return resource;
		}
	}

	return NULL;
}

void check_null_resource(t_resource* resource){
	if(resource == NULL){
			write_to_log(
				LOG_TARGET_INTERNAL,
				LOG_LEVEL_INFO,
				string_from_format("[utils/cpu-communication-utils - execute_kernel_signal] No existe el recurso")
				);
			//TODO: cambiar a exit y sacarlo de la lista
			exit(EXIT_FAILURE);
		}
}
