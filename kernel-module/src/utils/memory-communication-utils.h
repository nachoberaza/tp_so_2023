#ifndef UTILS_MEMORY_COMMUNICATION_UTILS_H_
#define UTILS_MEMORY_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/serialization/serialization.h>
	#include <commons/collections/list.h>
	#include "config-utils.h"
	#include "socket-utils.h"
	#include "logger-utils.h"
	#include "pcb-utils.h"
	#include "thread-utils.h"
	#include "cpu-communication-utils.h"
	#include "resource-utils.h"
	#include "semaphore-utils.h"

	t_list* create_segment_table();
	t_list* receive_segment_table();

#endif
