#include "nv_mm.h"
#include "nv_backend_nvdimm.h"
#include "common_config.h"

int main() {
	INIT_BACKEND();
	nv_mm_mmap("/tmp/mmap_test", mm_size, entry_count);
	char buf[100];
	
	nv_entry *e = (nv_entry *) nv_mm_get_addr();
	int i, used_count = 0, idx_count = 0, data_count = 0, key_data_count = 0;
	for (i = 0; i < entry_count; ++i, ++e) {
		if (!nv_entry_is_flag(e, NEF_PERSIST))
			continue;
		used_count++;	
		if (nv_entry_is_flag(e, NEF_DATA_TYPE)) {
			printf("%3d (%p):%s\n", NV_IDX(e), e, nv_data_str((nv_data *) e, buf, sizeof(buf))); fflush(stdout);
			if (nv_entry_is_flag(e, NEF_KEY))
				key_data_count++;
			else
				data_count++;
		} else {
			if (nv_idx_get_key_idx((nv_idx *) e) != 0) {
				printf("%3d (%p):%s\n", NV_IDX(e), e, nv_idx_str((nv_idx *) e, buf, sizeof(buf))); fflush(stdout);
				idx_count++;
			}
		}
	}

	printf("\nCurrent status\n");
	printf("\n- used: %d\n", used_count);
	printf("- unused: %d\n", nv_mm_get_unused_count());
	printf("- idx: %d\n- key: %d\n- data: %d\n", idx_count, key_data_count, data_count);
	return 0;
}
