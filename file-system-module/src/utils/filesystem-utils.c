#include "filesystem-utils.h"


void create_filesystem_structures(){;
	load_super_block();
	create_bitmap(get_file_system_config()->PATH_BITMAP, get_super_block_config()->BLOCK_COUNT);
	create_block_file();
	log_super_block(get_super_block_config());
}


void create_block_file(){
	t_log_level logLevel = LOG_LEVEL_INFO;
	int filePointer = open(get_file_system_config()->PATH_BLOQUES, O_CREAT | O_RDWR, 0664);

	if (filePointer == -1) {
		write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/filesystem - utils - create_block_file] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	int size = get_super_block_config()->BLOCK_COUNT * get_super_block_config()->BLOCK_SIZE;

	ftruncate(filePointer, size);
	close(filePointer);
}

