#include "kernel-communication-utils.h"

void fill_package_with_segment_table(t_package* pkg, t_list* segmentTable){
	int segmentTableCount = list_size(segmentTable);

	for (int i = 0; i < segmentTableCount; i++){
		t_segment_row* segment = list_get(segmentTable, i);
		fill_package_buffer(pkg, &segment->id, sizeof(int));
		fill_package_buffer(pkg, &segment->baseDirection, sizeof(int));
		fill_package_buffer(pkg, &segment->segmentSize, sizeof(int));
	}
}
