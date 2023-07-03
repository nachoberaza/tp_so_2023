#include "fcb-utils.h"

t_fcb* create_fcb_from_instruction(t_instruction* instruction){
	t_fcb* fcb = malloc(sizeof(t_fcb));
		fcb->fileName = list_get(instruction->parameters, 0);
		fcb->fileSize = 0;
		//fcb->directPointer = get_direct_pointer_from_instruction(instruction);
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

	write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/fcb-utils - log_fcb] Logeando FCB:\n");

	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("FileName: %s", fcb->fileName));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("FileSize: %i", fcb->fileSize));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("DirectPointer: %u", fcb->directPointer));
	write_to_log(LOG_TARGET_INTERNAL, logLevel, string_from_format("IndirectPointer %u \n", fcb->indirectPointer));
}

char *get_fcb_path(t_instruction* instruction){
	char* filePath = get_file_system_config()->PATH_FCB;
	char* fileName = list_get(instruction->parameters, 0);
	char* fileExtension = ".dat";

	// Obtener la longitud total de la cadena resultante
	size_t totalLength = strlen(filePath) + strlen(fileName) + strlen(fileExtension) + 1;

	// Asignar memoria para la cadena resultante
	char* filePathResult = malloc(totalLength);

	// Copiar las partes de la cadena en la memoria asignada
	strcpy(filePathResult, filePath);
	strcat(filePathResult, fileName);
	strcat(filePathResult, fileExtension);

	return filePathResult;

}

void persist_fcb(FILE *file,t_fcb *fcb){
	fprintf(file, "NOMBRE_ARCHIVO=%s \n",fcb->fileName);
	fprintf(file, "TAMANIO_ARCHIVO=%i \n",fcb->fileSize);
	fprintf(file, "PUNTERO_DIRECTO=%i \n",fcb->directPointer);
	fprintf(file, "PUNTERO_INDIRECTO=%i",fcb->indirectPointer);
}
