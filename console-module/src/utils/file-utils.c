#include "file-utils.h"

void read_console_commands(t_package* package) {
	char* readLine;
	int size;

	readLine = readline("> ");
	do {
		size = strlen(readLine) + 1;
		fill_buffer(package, readLine, size);

		readLine = readline("> ");
	}while (strcmp(readLine, "\0"));
}
