#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

#include <map>
#include <string>

//extern "C" {
	#include "nv_backend_nvdimm.h"
	#include "nv_backend_fs.h"
	#include "nv_map.h"
	#include "nv_mm.h"
	#include "nv_idx.h"
//}
#include "common_config.h"

int main() {
	int rc = 0, i;
	//char strbuf[4096];
	//char strbuf[792];
	//char strbuf[182];
	char strbuf[4];
	const char *key1 = "This_is_a_key1";
	const char *key2 = "This_is_a_key2";
	nv_map nvm;
	nv_idx_data in_data;
	char fmtbuf[30];
	
	const unsigned long int total_count = 1000000;
	//const unsigned long int total_count = 100;

	INIT_BACKEND();

	nv_mm_mmap("/tmp/mmap_test", mm_size, entry_count);
	nv_map_init(&nvm);
	printf("start\n");

	struct timespec tps, tpe;
	unsigned long int elapsed;

	repeat_str("data", strbuf, sizeof(strbuf));
	nv_idx_init_data(&in_data, (void *)strbuf, sizeof(strbuf));	
	clock_gettime(CLOCK_REALTIME, &tps);
	for (i = 0; i < total_count; ++i) {	
		rc = nv_map_put(&nvm, key1, &in_data);
		if (rc != 0) {
			if (rc == -ENOSPC) 
				fprintf(stderr, "nv_map_put error: no space\n");
			else
				fprintf(stderr, "nv_map_put error\n");
			goto mm_exit;
		}

		rc = nv_map_put(&nvm, key2, &in_data);
		if (rc != 0) {
			if (rc == -ENOSPC) 
				fprintf(stderr, "nv_map_put error: no space\n");
			else
				fprintf(stderr, "nv_map_put error\n");
			goto mm_exit;
		}
	}
	
	clock_gettime(CLOCK_REALTIME, &tpe);
	elapsed = ((tpe.tv_sec-tps.tv_sec) * 1000000000 + tpe.tv_nsec-tps.tv_nsec);

	printf("\nsize: %d, total count: %d\n", sizeof(strbuf), total_count);
	printf("elapsed: %lld, count: %lld\n", elapsed, total_count);
	printf("average latency: %.2f ns\n", (elapsed / (double) (total_count*2)));
	//printf("tt1: %.2f\n", (tt1 / (float) total_count));
	//printf("tt2: %.2f\n", (tt2 / (float) total_count));
	//printf("tt3: %.2f\n", (tt3 / (float) total_count));
	//printf("tt4: %.2f\n", (tt4 / (float) total_count));
mm_exit:
	nv_mm_unmap();
	return 0;
}
