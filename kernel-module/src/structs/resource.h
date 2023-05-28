#ifndef STRUCTS_RESOURCE_H_
#define STRUCTS_RESOURCE_H_

	typedef struct {
		char* name;
		int instances;
		t_list* blocked;
	} t_resource;

#endif
