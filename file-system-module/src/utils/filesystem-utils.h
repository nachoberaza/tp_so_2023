#ifndef UTILS_FILESYSTEM_UTILS_H_
#define UTILS_FILESYSTEM_UTILS_H_

	#include "./../file-system-module.h"
	#include <commons/bitarray.h>
	#include <sys/mman.h>
	#include <fcntl.h>
	#include "bitmap-utils.h"
	#include "fcb-utils.h"

	void create_filesystem_structures();
	void create_block_file();
#endif
