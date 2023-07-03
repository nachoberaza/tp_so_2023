#ifndef STRUCTS_FCB_H_
#define STRUCTS_FCB_H_
	#include <stdint.h>

 	typedef struct {
		char* fileName;
		int fileSize;
		uint32_t directPointer;
		uint32_t indirectPointer;
	} t_fcb;

#endif
