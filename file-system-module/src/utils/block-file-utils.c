#include "block-file-utils.h"

t_bitarray *blockfile;
const int emptyPointerValue = 40;
const int pointerLength = 4;

void create_block_file(){
	t_log_level logLevel = LOG_LEVEL_INFO;
	FILE *filePointer = fopen(get_file_system_config()->PATH_BLOQUES, "a+");

	int fileDescriptor = fileno(filePointer);

	if (fileDescriptor == -1) {
		write_to_log(LOG_TARGET_INTERNAL, logLevel, "[utils/filesystem - utils - create_block_file] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	int size = get_super_block_config()->BLOCK_COUNT * get_super_block_config()->BLOCK_SIZE;

	ftruncate(fileDescriptor, size);

	fclose(filePointer);
}

t_file_block* open_block_file(){
	t_file_block* fileBlock = malloc(sizeof(t_file_block));
	fileBlock->filePointer = fopen(get_file_system_config()->PATH_BLOQUES, "r+");

	int fileDescriptor = fileno(fileBlock->filePointer);

	if (fileDescriptor == -1) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/file-utils - open_block_file] Error abriendo archivo\n");
		exit(EXIT_FAILURE);
	}

	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	fileBlock->bmap = mmap(NULL, blockCount * blockSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

	if (fileBlock->bmap == MAP_FAILED) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, "[utils/file-utils - open_block_file] Error mapeando archivo\n");
		fclose(fileBlock->filePointer);
		exit(EXIT_FAILURE);
	}

	return fileBlock;
}

void write_in_block(t_instruction* instruction, char* value){
	char* fileName = list_get(instruction->parameters, 0);
	char* actualValue = value;
	int size = atoi(list_get(instruction->parameters, 2));
	int pointer = atoi(list_get(instruction->parameters, 4));

	t_fcb* fcb = get_fcb_by_name(fileName);

	int blockSize = get_super_block_config()->BLOCK_SIZE;
	int bytesToWrite;
	int bytesRemaining = size;

	for(int i = 0 ; bytesRemaining != 0; i++){
		int blockNumber = floor(pointer/blockSize) + i;
		int directPointer = get_pointer(fcb, blockNumber) * blockSize;

		if (i == 0) {
			bytesToWrite = (bytesRemaining < blockSize) ? bytesRemaining : blockSize - (pointer % blockSize);
		} else {
			bytesToWrite = (bytesRemaining < blockSize) ? bytesRemaining : blockSize;
		}

		if(i == 0 && pointer % blockSize != 0){
			directPointer += (pointer % blockSize);
		}
		bytesRemaining -= bytesToWrite;
		write_in_block_file(directPointer, actualValue, bytesToWrite);
		actualValue = string_substring_from(actualValue, bytesToWrite);
	}
}

int get_pointer(t_fcb* fcb, int blockNumber){
	const int pointerSize = 4;
	if (blockNumber == 0)
		return fcb->directPointer;

	// OFFSET DE BLOQUE DE PUNTEROS
	int offset = pointerSize * (blockNumber - 1);
	// EXTRAE EL PUNTERO
	uint32_t value = extract_uint32_from_block(fcb->indirectPointer *  get_super_block_config()->BLOCK_SIZE, offset);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/file-utils - get_pointer] Se obtuvo el valor: %d\n", value));
	return (int)value;
}


void write_in_block_file(int directPointer, void* value, int size){
	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	t_file_block* fileBlock = open_block_file();
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, string_from_format("[utils/file-utils - write_in_block_file] DirectPointer: %d", directPointer));

	char* currentAddress = fileBlock->bmap + directPointer;

	memcpy(currentAddress, value, size);
	//fflush(fileBlock->filePointer);

	fclose(fileBlock->filePointer);
	munmap(fileBlock->bmap, blockCount * blockSize);
}

char* read_block(t_instruction* instruction){
	char* fileName = list_get(instruction->parameters, 0);
	int size = atoi(list_get(instruction->parameters, 2));
	int pointer = atoi(list_get(instruction->parameters, 4));
	t_fcb* fcb = get_fcb_by_name(fileName);

	char* readValue = string_new();
	int blockSize = get_super_block_config()->BLOCK_SIZE;
	int bytesToRead;
	int bytesRemaining = size;

	for(int i = 0 ; bytesRemaining != 0; i++){
		int blockNumber = floor(pointer/blockSize) + i;
		int directPointer = get_pointer(fcb, blockNumber) * blockSize;

		if (i == 0) {
			bytesToRead = (bytesRemaining < blockSize) ? bytesRemaining : blockSize - (pointer % blockSize);
		} else {
			bytesToRead = (bytesRemaining < blockSize) ? bytesRemaining : blockSize;
		}

		if(i == 0 && pointer % blockSize != 0){
			directPointer += (pointer % blockSize);
		}
		bytesRemaining -= bytesToRead;
		string_append(&readValue, read_block_file(directPointer, bytesToRead));
	}
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR, string_from_format("[utils/file-utils - read_block] Valor leido de file: %s", readValue));
	return readValue;
}


char* read_block_file(int directPointer, int size){
	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;
	char* returnValue = string_new();
	t_file_block* fileBlock = open_block_file();
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_DEBUG, string_from_format("[utils/file-utils - read_block_file] DirectPointer: %d", directPointer));

	char* currentAddress = fileBlock->bmap + directPointer;

	memcpy(returnValue, currentAddress, size);
	returnValue = string_substring(returnValue, 0, size);
	fclose(fileBlock->filePointer);
	munmap(fileBlock->bmap, blockCount * blockSize);
	return returnValue;
}


uint32_t extract_uint32_from_block(int indirectPointer, int offset){
	int blockCount = get_super_block_config()->BLOCK_COUNT;
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	t_file_block* fileBlock = open_block_file();
	uint32_t value = fileBlock->bmap[indirectPointer + offset];

	fclose(fileBlock->filePointer);
	munmap(fileBlock->bmap, blockCount * blockSize);
	return value;
}


void truncate_file(t_fcb* fcb, int size){
	int fcbCurrentSize = get_fcb_size(fcb);
	int blockSize = get_super_block_config()->BLOCK_SIZE;

	int blockCount = floor(size / blockSize);
	if (size % blockSize != 0)
		blockCount++;

	for(int i=fcbCurrentSize; i<blockCount; i++){
		assign_new_block(fcb, i);
	}

	int maxBlockSize = blockSize / 4;


	for(int i=blockCount; i<maxBlockSize; i++){
		int position = (fcb->indirectPointer * blockSize) + (i - 1) * pointerLength;
		write_in_block_file(position, &emptyPointerValue, pointerLength);
	}

	//persist_fcb(fcb);
}

void assign_new_block(t_fcb* fcb, int currentSize){
	uint32_t block = (uint32_t)get_first_empty_block();
	if (currentSize == 0)
	{
		fcb->directPointer = block;
		mark_block_as_used(block);
		return;
	}
	else if (currentSize == 1){
		fcb->indirectPointer = block;
		mark_block_as_used(block);
	}
	block = (uint32_t)get_first_empty_block();

	int directPointer = fcb->indirectPointer;

	int position = (directPointer * get_super_block_config()->BLOCK_SIZE) + (currentSize - 1) * pointerLength;

	write_in_block_file(position, &block, pointerLength);
	mark_block_as_used(block);
}

int get_fcb_size(t_fcb *fcb){
	const int pointerSize = 4;
	int size = 0;
	if (!is_block_used(fcb->directPointer))
		return 0;
	size++;

	int offset = 0;
	uint32_t value = 0;
	int currentPointer = 0;

	while (1){
		offset = pointerSize * currentPointer;
		value = (int) extract_uint32_from_block(fcb->indirectPointer, offset);

		if (value == emptyPointerValue)
			return size;

		if (!is_block_used(value))
			return size;

		size++;
	}
}

