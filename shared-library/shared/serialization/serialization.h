#ifndef SHARED_SERIALIZATION_H_
#define SHARED_SERIALIZATION_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include "../execution-context/execution-context.h"
	#include "../logs/log.h"
	#include "../structs/execution-context.h"

	command extract_command_from_buffer(void* buffer, int* offset);
	char* extract_string_from_buffer(void* buffer, int* offset);
	void copy_string_from_buffer_into_variable(char* destiny, void* buffer, int* offset);
	int extract_int_from_buffer(void* buffer, int* offset);

#endif
