#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

//extern "C" {
	#include "nv_backend_nvdimm.h"
	#include "nv_map.h"
	#include "nv_mm.h"
	#include "nv_idx.h"
//}
#include "common_config.h"

int main(int argc, char *argv[]) {
	int rc = 0;
	const char* key = argv[1];
	const char* data = argv[2];

	if (argc != 3) {
		fprintf(stderr, "Key name and data is required\n");
		return 0; 
	}

	INIT_BACKEND();

	nv_idx_data in_data, *out_data;
	nv_map nvm;
	char fmtbuf[100];

	nv_mm_mmap("/tmp/mmap_test", mm_size, entry_count);
	nv_map_init(&nvm);

	nv_idx_init_data(&in_data, (void *)data, strlen((char *)data));

	rc = nv_map_put(&nvm, key, &in_data);
	if (rc < 0) {
		if (rc == -ENOSPC) 
			fprintf(stderr, "nv_map_put error: no space\n");
		else
			fprintf(stderr, "nv_map_put error\n");
	}

	nv_mm_unmap();
	return rc;
}
