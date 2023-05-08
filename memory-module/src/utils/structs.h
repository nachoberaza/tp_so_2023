#ifndef UTILS_STRUCTS_H_
#define UTILS_STRUCTS_H_
	#include <shared/structs/socket.h>
	#define MODULE_ENUM_SIZE 3

	static char * moduleNames[MODULE_ENUM_SIZE] = {
			[KERNEL] = "KERNEL",
			[CPU] = "CPU",
			[FILESYSTEM] = "FILESYSTEM",
		};
#endif
