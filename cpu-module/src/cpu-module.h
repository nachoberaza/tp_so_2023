#ifndef CPU_MODULE_H_
#define CPU_MODULE_H_

#define MODULE_NAME "cpu"

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include <shared/execution-context/execution-context.h>
	#include <shared/serialization/serialization.h>
	#include "utils/config-utils.h"
	#include "utils/logger-utils.h"
	#include "utils/execution-context-utils.h"
	#include "utils/commands-utils.h"

	//Only for sleep, must be removed eventually
	#include <unistd.h>



#endif
