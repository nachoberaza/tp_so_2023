#include "thread-utils.h"

pthread_t listenerThread;

void create_listener_thread(int serverSocketId){
	int threadStatus = pthread_create(&listenerThread, NULL, listen_consoles, serverSocketId);
    if (threadStatus != 0){
    	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("Hubo un problema al crear el listener thread: [%s]", strerror(threadStatus)));
    	exit(EXIT_FAILURE);
    }
}

pthread_t get_listener_thread(){
	return listenerThread;
}
