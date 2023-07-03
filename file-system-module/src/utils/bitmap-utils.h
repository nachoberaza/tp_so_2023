
#ifndef UTILS_BITMAP_UTILS_H_
#define UTILS_BITMAP_UTILS_H_
	#include "./../file-system-module.h"
	#include <commons/bitarray.h>
	#include <sys/mman.h>
	#include <fcntl.h>

	void create_bitmap(char* archivo, size_t size);
	t_bitarray* get_bitmap();
	off_t get_first_empty_block();
	void mark_block_as_used(off_t index);
	void mark_block_as_unused(off_t index);

#endif
