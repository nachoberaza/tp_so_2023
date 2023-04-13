#include "file-utils.h"

void fill_package_from_file(t_package *package){
	FILE * filePointer;
	char * line = NULL;
	size_t length = 0;
	ssize_t read;

	filePointer = fopen("console.code", "r");
	if (filePointer == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &length, filePointer)) != -1) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Linea: %s", line));
		fill_package(package, line, read);
	}

	fclose(filePointer);
	if (line)
		free(line);
}
