#include "file-utils.h"

void fill_buffer_from_file(t_buffer* buffer){
	FILE * file;
	char * line = NULL;
	// Variable para uso interno de getline
	size_t bufferSize = 0;
	ssize_t lineLength;

	file = fopen("console.code", "r");

	if (file == NULL)
		exit(EXIT_FAILURE);

	while ((lineLength = getline(&line, &bufferSize, file)) != -1) {
		if (string_ends_with(line,"\n")){
			line = string_substring(line, 0, lineLength-1);
		}else
			lineLength++;

		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/file-utils - fill_buffer_from_file] Valor de la linea: %s", line));
		fill_buffer(buffer, line, lineLength);
	}

	fclose(file);
	if (line)
		free(line);
}
