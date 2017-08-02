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
	if (argc != 2) {
		fprintf(stderr, "the key name is required\n");
		return 0; 
	}
	const char* key_name = argv[1];

	INIT_BACKEND();

	nv_idx_data *out_data;
	nv_map nvm;
	char fmtbuf[100];

	nv_mm_mmap("/tmp/mmap_test", mm_size, entry_count);
	nv_map_init(&nvm);

	if((out_data = nv_map_get(&nvm, key_name)) != NULL) {
		if (out_data->size > 0) {
			sprintf(fmtbuf, "%%.%lds\n", out_data->size, out_data->size);
			//sprintf(fmtbuf, "Found: %%.%lds(%ld)\n", out_data->size, out_data->size);
			printf(fmtbuf, (char *)out_data->data);
		} else {
			printf("NULL\n");
		}
	} else {
		printf("Not found: %s\n", key_name);
	}
	nv_mm_unmap();
	return 0;
}
