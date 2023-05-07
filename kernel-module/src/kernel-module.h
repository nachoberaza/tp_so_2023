#ifndef KERNEL_MODULE_H_
#define KERNEL_MODULE_H_

#define MODULE_NAME "kernel"


	#include <stdio.h>
	#include <stdlib.h>
	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include <shared/sockets/server-socket.h>
	#include <shared/sockets/client-socket.h>
	#include "utils/config-utils.h"
	#include "utils/socket-utils.h"
	#include "utils/logger-utils.h"
	#include "utils/pcb-utils.h"


	void send_package_to_all_targets(t_list *commands, t_kernel_connections* KERNEL_CONNECTIONS);
	void build_pcb(t_list *commands);
	void handle_console(t_kernel_connections* KERNEL_CONNECTIONS, int serverSocketId);

#endif
