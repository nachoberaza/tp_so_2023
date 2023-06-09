#ifndef STRUCTS_CONFIG_H_
#define STRUCTS_CONFIG_H_

	#define PLANNING_ALGORITHM_ENUM_SIZE 2

	typedef enum {
		FIFO, HRRN
	} planning_algorithm;

	static char * planningAlgorithmNames[PLANNING_ALGORITHM_ENUM_SIZE] = {
		[FIFO] = "FIFO",
		[HRRN] = "HRRN",
	};


#endif
