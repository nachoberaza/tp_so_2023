#ifndef KERNEL_MODULE_H_
#define KERNEL_MODULE_H_

#define MODULE_NAME "kernel"

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/serialization/serialization.h>
	#include "utils/config-utils.h"
	#include "utils/socket-utils.h"
	#include "utils/logger-utils.h"
	#include "utils/pcb-utils.h"
	#include "utils/thread-utils.h"
	#include "utils/cpu-communication-utils.h"
	#include "utils/resource-utils.h"
	#include "utils/semaphore-utils.h"

#endif
