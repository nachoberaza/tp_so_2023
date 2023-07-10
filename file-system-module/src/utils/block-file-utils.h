#ifndef UTILS_BLOCK_FILE_UTILS_H_
#define UTILS_BLOCK_FILE_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/bitarray.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <sys/mman.h>
	#include <fcntl.h>
	#include <string.h>
	#include <math.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/memory/memory.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/logs/log.h>
 	#include "config-utils.h"
	#include "fcb-utils.h"
	#include "bitmap-utils.h"
	#include "config-utils.h"
	#include "logger-utils.h"

	typedef struct {
		FILE* filePointer;
		char* bmap;
	}t_file_block;

	void create_block_file();
	t_file_block* open_block_file();
	int get_pointer(t_fcb* fcb, int blockNumber);
	void write_in_block_file(int directPointer, void* value, int size);
	void write_in_block(t_instruction* instruction, char* value);
	uint32_t extract_uint32_from_block(int indirectPointer, int offset);
	void truncate_file(t_fcb* fcb, int size);
	void assign_new_block(t_fcb* fcb, int currentSize);
	int get_fcb_size(t_fcb *fcb);

#endif
