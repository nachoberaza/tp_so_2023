#include "file-utils.h"

void fill_buffer_from_file(t_buffer* buffer){
	FILE * file;
	char * line = NULL;
	//variable para uso interno del getLine, no viene al caso y no deberiamos usarla
	size_t bufferSize = 0;
	ssize_t lineLength;

	file = fopen("console.code", "r");
	if (file == NULL)
		exit(EXIT_FAILURE);

	while ((lineLength = getline(&line, &bufferSize, file)) != -1) {
		if (string_ends_with(line,"\n")){
			line = string_substring(line, 0, lineLength-1);
		}

		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Line value: %s", line));
		fill_buffer_with_string(buffer, line, lineLength);
	}

	fclose(file);
	if (line)
		free(line);
}
