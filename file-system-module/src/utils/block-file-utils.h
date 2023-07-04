#ifndef UTILS_BLOCK_FILE_UTILS_H_
#define UTILS_BLOCK_FILE_UTILS_H_

	#include "./../file-system-module.h"
	#include <commons/bitarray.h>
	#include <sys/mman.h>
	#include <fcntl.h>

	void create_block_file();
	char* open_block_file(FILE **filePointer);
	int get_pointer(t_fcb* fcb, int blockNumber);
	void write_in_block_file(int directPointer, char* value, int size);
	void write_in_block(t_instruction* instruction, char* value);
	int extract_from_block(int indirectPointer, int offset, int size);

#endif
