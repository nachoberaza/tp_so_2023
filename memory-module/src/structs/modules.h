#ifndef STRUCTS_MODULES_H_
#define STRUCTS_MODULES_H_
	#include <shared/structs/socket.h>
	#define MODULE_ENUM_SIZE 3

	static char * moduleNames[MODULE_ENUM_SIZE] = {
			[KERNEL] = "KERNEL",
			[CPU] = "CPU",
			[FILESYSTEM] = "FILESYSTEM",
		};

	typedef struct {
		pthread_t kernelThread;
		pthread_t cpuThread;
		pthread_t fileSystemThread;
	} t_modules_thread_id;

#endif
