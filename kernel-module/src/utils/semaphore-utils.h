#ifndef UTILS_SEMAPHORE_UTILS_H_
#define UTILS_SEMAPHORE_UTILS_H_

	#include <semaphore.h>

	#include <commons/log.h>
	#include <commons/string.h>
	#include <shared/logs/log.h>
	#include "logger-utils.h"

	void init_semaphores();
	void wait_short_term();
	void signal_short_term();
	void wait_long_term();
	void signal_long_term();

#endif
