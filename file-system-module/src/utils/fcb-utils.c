#include "fcb-utils.h"

t_list* fcbList;

void load_fcb_list(){
	fcbList = list_create();

    DIR *dir;
    struct dirent *entry;

    dir = opendir(get_file_system_config()->PATH_FCB);

    if (dir == NULL) {
    	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/fcb-utils - create_FCB_list] No se pudo abrir el directorio de FCB");
    }

    while ((entry = readdir(dir)) != NULL) {
    	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
    	    continue;
    	}
    	t_fcb* fcb = create_fcb_from_path(entry->d_name);
    	list_add(fcbList, fcb);
    }

    log_fcb_list(fcbList);

    closedir(dir);
}

t_fcb* get_fcb_by_name(char* name){
	int size = list_size(fcbList);

	for (int i = 0; i < size; i++){
		t_fcb* fcb = list_get(fcbList, i);
		if (string_equals_ignore_case(fcb->fileName, name)){
			return fcb;
		}
	}

	return NULL;
}

t_list* get_fcb_list(){
	return fcbList;
}

t_fcb* create_fcb_from_path(char* name){
	char* path = string_from_format("%s/%s", get_file_system_config()->PATH_FCB, name);

	t_fcb* fcb = malloc(sizeof(t_fcb));

	t_config *config = config_create(path);

	char* untrimmedName = string_duplicate(config_get_string_value(config, "NOMBRE_ARCHIVO"));
	string_trim(&untrimmedName);
	fcb->fileName = untrimmedName;
	fcb->fileSize = config_get_int_value(config, "TAMANIO_ARCHIVO");
	fcb->directPointer = (uint32_t) config_get_int_value(config, "PUNTERO_DIRECTO");
	fcb->indirectPointer = (uint32_t) config_get_int_value(config, "PUNTERO_INDIRECTO");

	config_destroy(config);

	return fcb;
}

t_fcb* create_fcb_from_instruction(t_instruction* instruction){
	t_fcb* fcb = malloc(sizeof(t_fcb));
	fcb->fileName = list_get(instruction->parameters, 0);
	fcb->fileSize = 0;
	fcb->directPointer = 0;
	fcb->indirectPointer = 0;
	return fcb;
}

uint32_t get_direct_pointer_from_instruction(t_instruction* instruction){
	uint32_t directPointer = strtoul(list_get(instruction->parameters, 1),NULL,10);
	return directPointer;
}

void log_fcb(t_fcb* fcb) {
	t_log_level logLevel = LOG_LEVEL_TRACE;

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/fcb-utils - log_fcb] Logging FCB:\n");

	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("FileName: %s", fcb->fileName));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("FileSize: %i", fcb->fileSize));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("DirectPointer: %u", fcb->directPointer));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("IndirectPointer %u \n", fcb->indirectPointer));
}

void log_fcb_list(t_list* fcbList){
	int size = list_size(fcbList);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_TRACE, string_from_format("[utils/fcb-utils - log_fcb_list] Logeando %d elementos en FCB list :\n", size));
	for (int i = 0; i < size; i++){
		t_fcb* fcb = list_get(fcbList, i);
		log_fcb(fcb);
	}
}

char *get_fcb_path(t_instruction* instruction){
	char* filePath = get_file_system_config()->PATH_FCB;
	char* fileName = list_get(instruction->parameters, 0);
	char* fileExtension = ".dat";

	// Obtener la longitud total de la cadena resultante
	size_t totalLength = strlen(filePath) + strlen("/") + strlen(fileName) + strlen(fileExtension) + 1;

	// Asignar memoria para la cadena resultante
	char* filePathResult = malloc(totalLength);

	// Copiar las partes de la cadena en la memoria asignada
	strcpy(filePathResult, filePath);
	strcat(filePathResult, "/");
	strcat(filePathResult, fileName);
	strcat(filePathResult, fileExtension);

	return filePathResult;

}

void persist_fcb(FILE *file,t_fcb *fcb){
	//Cambiar a update fcb, q abra el archivo con el nombre, lo edite y lo cierre
	fprintf(file, "NOMBRE_ARCHIVO=%s \n",fcb->fileName);
	fprintf(file, "TAMANIO_ARCHIVO=%i \n",fcb->fileSize);
	fprintf(file, "PUNTERO_DIRECTO=%i \n",fcb->directPointer);
	fprintf(file, "PUNTERO_INDIRECTO=%i",fcb->indirectPointer);
}
