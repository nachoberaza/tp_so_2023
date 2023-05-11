#ifndef UTILS_THREAD_UTILS_H_
#define UTILS_THREAD_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include "socket-utils.h"

	void create_listener_thread(int serverSocketId);
	pthread_t get_listener_thread();

#endif
