#include "block-file-utils.h"

t_bitarray *blockfile;

void create_block_file(){
	t_log_level logLevel = LOG_LEVEL_INFO;
	FILE *filePointer = fopen(get_file_system_config()->PATH_BLOQUES, "w+");

	int fileDescriptor = fileno(filePointer);

	if (fileDescriptor == -1) {
		write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/filesystem - utils - create_block_file] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	int size = get_super_block_config()->BLOCK_COUNT * get_super_block_config()->BLOCK_SIZE;

	ftruncate(fileDescriptor, size);

	fclose(filePointer);
}

char* open_block_file(FILE **filePointer){

	*filePointer = fopen(get_file_system_config()->PATH_BLOQUES, "r+");

	int fileDescriptor = fileno(filePointer);

	if (fileDescriptor == -1) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/file-utils - extract_from_block] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	char* bmap = mmap(NULL, blockCount * blockSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

	if (bmap == MAP_FAILED) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/file-utils - extract_from_block] Error mapeando archivo\n");
		fclose(filePointer);
		exit(EXIT_FAILURE);
	}
}

void write_in_block(t_instruction* instruction, char* value){
	char* fileName = list_get(instruction->parameters, 0);
	int size = atoi(list_get(instruction->parameters, 2));
	int pointer = atoi(list_get(instruction->parameters, 4));


	t_fcb* fcb = get_fcb_by_name(fileName);

	int blockSize = get_super_block_config()->BLOCK_SIZE;

	int blockNumber = floor(pointer/blockSize);

	int directPointer = get_pointer(fcb, blockNumber);

	write_in_block_file(directPointer, value, size);
}

void write_in_block_file(int directPointer, char* value, int size){
	FILE *filePointer;
	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	char* bmap = open_block_file(&filePointer);

	strncpy(bmap, value, size);

	fclose(filePointer);
	munmap(bmap, blockCount * blockSize);
}

int get_pointer(t_fcb* fcb, int blockNumber){
	const int pointerSize = 4;
	if (blockNumber == 0)
		return fcb->directPointer;


	int offset = pointerSize * (blockNumber - 1);
	int value = extract_from_block(fcb->indirectPointer, offset, 4);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/file-utils - get_pointer] Se obtuvo el valor: %d\n", value));
	return value;
}

int extract_from_block(int indirectPointer, int offset, int size){
	FILE *filePointer = fopen(get_file_system_config()->PATH_BLOQUES, "r");

	int fileDescriptor = fileno(filePointer);

	if (fileDescriptor == -1) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/file-utils - extract_from_block] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	char* bmap = mmap(NULL, blockCount * blockSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

	if (bmap == MAP_FAILED) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/file-utils - extract_from_block] Error mapeando archivo\n");
		fclose(filePointer);
		exit(EXIT_FAILURE);
	}
	int value = bmap[indirectPointer + offset];

	fclose(filePointer);
	munmap(bmap, blockCount * blockSize);
	return value;
}

