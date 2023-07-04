#include "filesystem-utils.h"


void create_filesystem_structures(){
	load_super_block();
	load_fcb_list();
	create_bitmap(get_file_system_config()->PATH_BITMAP, get_super_block_config()->BLOCK_COUNT);
	create_block_file();
}
