#include "file-utils.h"

void fill_package_from_file(t_package *package){
	FILE * file;
	char * line = NULL;
	size_t tSize = 0;
	ssize_t length;

	file = fopen("console.code", "r");
	if (file == NULL)
		exit(EXIT_FAILURE);

	while ((length = getline(&line, &tSize, file)) != -1) {
		if (string_ends_with(line,"\n")){
			line = string_substring(line,0,length-1);
		}

		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("Line value: %s", line));
		fill_buffer(package, line, length);
	}

	fclose(file);
	if (line)
		free(line);
}
