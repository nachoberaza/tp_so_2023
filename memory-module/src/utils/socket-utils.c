#include "socket-utils.h"

t_modules_thread_id *modulesThreadId;

void init_modules(){
	modulesThreadId=malloc(sizeof(t_modules_thread_id));
}

t_modules_thread_id *get_modules_thread_id(){
	return modulesThreadId;
}

void handle_handshake(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Iniciando Handshake...");
	int handshake = receive_handshake(clientSocketId);
	switch (handshake) {
	case KERNEL:
		accept_module(clientSocketId, handshake);
		handle_kernel_connection(clientSocketId);
		break;
	case CPU:
		accept_module(clientSocketId, handshake);
		handle_cpu_connection(clientSocketId);
		break;
	case FILESYSTEM:
		accept_module(clientSocketId, handshake);
		handle_filesystem_connection(clientSocketId);
		break;
	default:;
		operation_result response= OPERATION_RESULT_ERROR;
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_WARNING,"Un modulo desconocido intento conectarse");
		send(clientSocketId, &response, sizeof(operation_result), NULL);
		exit(EXIT_FAILURE);
	}
}

char *module_as_string(module_handshakes module) {
	return moduleNames[module];
}

void accept_module(int clientSocketId, int module) {
	operation_result result = (operation_result) OPERATION_RESULT_OK;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,string_from_format("Modulo %s aceptado",module_as_string(module)));
}

void handle_kernel_connection(int clientSocketId) {
	int err = pthread_create(&modulesThreadId->kernelThread, NULL, listen_kernel_connection,clientSocketId);
	if (err != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error creating thread to listen kernel");
		exit(EXIT_FAILURE);
	}
}

void handle_cpu_connection(int clientSocketId) {
	int err = pthread_create(&modulesThreadId->cpuThread, NULL, listen_cpu_connection,clientSocketId);
	if (err != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error creating thread to listen cpu");
		exit(EXIT_FAILURE);
	}
}
void handle_filesystem_connection(int clientSocketId) {
	int err = pthread_create(&modulesThreadId->fileSystemThread, NULL, listen_filesystem_connection,clientSocketId);
	if (err != 0) {
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_ERROR,"Error creating thread to listen filesystem");
		exit(EXIT_FAILURE);
	}
}

void listen_kernel_connection(int clientSocketId) {
	while(1){
		sleep(30);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Listen Kernel Connection");
	}
}

void listen_cpu_connection(int clientSocketId) {
	while(1){
		sleep(30);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Listen CPU Connection");
	}
}

void listen_filesystem_connection(int clientSocketId) {
	while(1){
		sleep(30);
		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "Listen Filesystem Connection");
	}
}
