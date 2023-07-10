#include "cpu-communication-utils.h"

void listen_cpu_connection(int clientSocketId) {
	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_cpu_connection] Escuchando al modulo cpu");
	while (1) {
		int opCode = receive_operation_code(clientSocketId);

		write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, string_from_format("[utils/socket-utils - listen_cpu_connection] Recibi operation code %d", opCode));

		t_memory_data* data = decode_memory_data(get_logger(),clientSocketId);

		log_memory_data(data,get_logger(), LOG_LEVEL_INFO);

		switch (data->instruction->command){
			case MOV_IN:
				execute_memory_mov_in(data, clientSocketId,"CPU"); //El origen es al pedo mandarlo en la data porque tenemos listen de CPU y FS
				break;
			case MOV_OUT:
				execute_memory_mov_out(data, clientSocketId,"CPU");
				break;
			default:
				write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO, "[utils/socket-utils - listen_cpu_connection] Comando no pertenece a Memory");
				return;
			break;
		}
	}
}

void execute_memory_mov_in(t_memory_data* data, int clientSocketId, char* origin){
	int address = atoi(list_get(data->instruction->parameters, 0));
	int size = atoi(list_get(data->instruction->parameters, 1));

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO,
			string_from_format("PID: %d - Acción: LEER - Dirección física: %d - Tamaño: %d - Origen: %s",data->pid, address, size, origin));

	char* value = malloc(size);

	sleep(get_memory_config()->MEMORY_DELAY);

	memcpy(value, get_memory() + address, size);

	write_to_log(LOG_TARGET_INTERNAL, LOG_LEVEL_INFO,
			string_from_format("MOV_IN Value: %s",value));

	t_package* package = create_package();

	fill_buffer(package->buffer, value, size);
	send_package(package, clientSocketId);
}

void execute_memory_mov_out(t_memory_data* data, int clientSocketId,char* origin){
	int address = atoi(list_get(data->instruction->parameters, 0));
	char* value = list_get(data->instruction->parameters, 1);
	int size = atoi(list_get(data->instruction->parameters, 2));

	write_to_log(LOG_TARGET_MAIN, LOG_LEVEL_INFO,
				string_from_format("PID: %d - Acción: ESCRIBIR - Dirección física: %d - Tamaño: %d - Origen: %s",data->pid,address,size,origin));

	void* destination = get_memory() + address;

	sleep(get_memory_config()->MEMORY_DELAY);

	memcpy(destination, value, strlen(value));

	operation_result result;
	send(clientSocketId, &result, sizeof(operation_result), NULL);
}
