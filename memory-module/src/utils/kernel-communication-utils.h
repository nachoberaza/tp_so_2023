#ifndef UTILS_KERNEL_COMMUNICATION_UTILS_H_
#define UTILS_KERNEL_COMMUNICATION_UTILS_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/structs/memory.h>
	#include <shared/structs/socket.h>
	#include <shared/sockets/client-socket.h>
	#include "config-utils.h"
	#include "logger-utils.h"

	void fill_package_with_segment_table(t_package* pkg, t_list* segmentTable);
#endif
