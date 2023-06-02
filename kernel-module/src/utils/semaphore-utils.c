#include "semaphore-utils.h"

sem_t shortTerm;
sem_t longTerm;

void init_semaphores(){
	  sem_init(&shortTerm , 1, 1);
	  sem_init(&longTerm , 1, 1);
}

void wait_short_term(){
	int value;
	sem_getvalue(&shortTerm , &value);
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			string_from_format("[utils/semaphore-utils - wait_short_term] Esperando, valor actual: %d", value)
	);

	sem_wait(&shortTerm);
}

void signal_short_term(){
	int value;
	sem_getvalue(&shortTerm , &value);
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			string_from_format("[utils/semaphore-utils - signal_short_term] Signaling, valor actual: %d", value)
	);

	sem_post(&shortTerm);
}

void wait_long_term(){
	int value;
	sem_getvalue(&longTerm , &value);
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			string_from_format("[utils/semaphore-utils - wait_long_term] Esperando, valor actual: %d", value)
	);

	sem_wait(&longTerm);

}

void signal_long_term(){
	int value;
	sem_getvalue(&longTerm , &value);
	write_to_log(
			LOG_TARGET_INTERNAL,
			LOG_LEVEL_TRACE,
			string_from_format("[utils/semaphore-utils - signal_long_term] Signaling, valor actual: %d", value)
	);

	sem_post(&longTerm);
}
