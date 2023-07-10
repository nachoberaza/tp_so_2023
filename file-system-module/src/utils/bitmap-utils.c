#include "bitmap-utils.h"

t_bitarray *bitmap;

void create_bitmap(char* file, size_t size){
	t_log_level logLevel = LOG_LEVEL_INFO;
	FILE *filePointer = fopen(file, "a+");

	int fileDescriptor = fileno(filePointer);

	if (fileDescriptor == -1) {
		write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/filesystem - utils - create_bitmap] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	ftruncate(fileDescriptor, size);

	char* bmap = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

	if (bmap == MAP_FAILED) {
		write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/filesystem - utils - create_bitmap] Error mapeando archivo\n");
		fclose(filePointer);
		exit(EXIT_FAILURE);
	}

	bitmap = bitarray_create_with_mode(bmap, size, LSB_FIRST);

	fclose(filePointer);
	msync(file, size , MS_SYNC);
}

t_bitarray *get_bitmap(){
	return bitmap;
}

off_t get_first_empty_block(){
	t_bitarray *bitmapToCheck = get_bitmap();

	for(off_t index = 0; index < (bitmapToCheck->size);index++){
		if(!bitarray_test_bit(bitmapToCheck,index)){
			return index;
		}
	}
	return -1;
}

void mark_block_as_used(off_t index){
	 bitarray_set_bit(get_bitmap(), index);
}

void mark_block_as_unused(off_t index){
	 bitarray_clean_bit(get_bitmap(), index);
}

int is_block_used(int index){
	return bitarray_test_bit(get_bitmap(), index);
}
