#include "serialization.h"

void copy_string_from_buffer_into_variable(char* destiny, void* buffer, int* offset) {
	int originalOffset = *offset;
	char* data = extract_string_from_buffer(buffer, offset);
	int length = (*offset) - originalOffset - sizeof(int);
	strncpy(destiny, data, length);
}

char* extract_string_from_buffer(void* buffer, int* offset){
	int valueSize = 0;
	memcpy(&valueSize, buffer + *offset, sizeof(int));
	*offset += sizeof(int);
	char *value = malloc(valueSize);
	memcpy(value, buffer + *offset, valueSize);
	*offset += valueSize;

	return value;
}

command extract_command_from_buffer(void* buffer, int* offset){
	int valueSize = 0;
	command returnValue;
	memcpy(&valueSize, buffer + *offset, sizeof(int));
	*offset += sizeof(int);
	memcpy(&returnValue, buffer + *offset, sizeof(command));
	*offset += sizeof(int);

	return returnValue;
}

exit_reason extract_exit_reason_from_buffer(void* buffer, int* offset){
	int valueSize = 0;
	exit_reason returnValue;
	memcpy(&valueSize, buffer + *offset, sizeof(int));
	*offset += sizeof(int);
	memcpy(&returnValue, buffer + *offset, sizeof(exit_reason));
	*offset += sizeof(int);

	return returnValue;
}

int extract_int_from_buffer(void* buffer, int* offset){
	int valueSize = 0, returnValue;
	memcpy(&valueSize, buffer + *offset, sizeof(int));
	*offset += sizeof(int);
	memcpy(&returnValue, buffer + *offset, sizeof(int));
	*offset += sizeof(int);

	return returnValue;
}
