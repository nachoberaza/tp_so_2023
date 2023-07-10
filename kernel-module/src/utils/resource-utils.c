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

t_resource* get_resource(t_list * list,char* resourceName){
	int size = list_size(list);

	for(int i=0; i< size;i++){
		t_resource* resource = list_get(list,i);

		if(!strcmp(resource->name,resourceName)){
			return resource;
		}
	}

	return NULL;
}

operation_result check_null_resource(t_resource* resource,t_pcb* pcb){
	if(resource == NULL){
		write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_ERROR,
			string_from_format("[utils/resources-utils - check_null_resource] No existe el recurso")
			);

		add_error_in_execution_context_reason(pcb->executionContext->reason , REASON_ERROR , ERROR_INVALID_RESOURCE);
		move_to_exit(pcb);
		return OPERATION_RESULT_ERROR;
	}

	return OPERATION_RESULT_OK;
}

int get_resource_index(t_list * list,char* resourceName){
	int size = list_size(list);

	write_to_log(
		LOG_TARGET_INTERNAL,
		LOG_LEVEL_INFO,
		string_from_format("[utils/resources-utils - get_resource_index] Recurso a buscar : %s",resourceName)
	);

	for(int i=0; i< size;i++){
		t_resource* resource = list_get(list,i);

		write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_INFO,
			string_from_format("[utils/resources-utils - get_resource_index] Nombre recurso encontrado: %s",resource->name)
		);


		if(!strcmp(resource->name,resourceName)){
			return i;
		}
	}

	return -1;
}
