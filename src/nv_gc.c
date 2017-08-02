#include "nv_gc.h"
#include "nv_mm.h"

//#define GC_DEBUG

void _gc(nv_entry *e) {
	char buf[100];
	if (!nv_entry_is_flag(e, NEF_PERSIST)) {
		return;
	}
	#if defined GC_DEBUG
	printf("valid entry: %d\n", NV_IDX(e));
	#endif

	if (nv_entry_is_flag(e, NEF_DATA_TYPE)) {
		#if defined GC_DEBUG
		printf("free data1: %d %s\n", NV_IDX(e), nv_data_str((nv_data *)e, buf, sizeof(buf)));
		#endif

		nv_data *d = (nv_data *) NV_ENTRY(nv_data_get_next_idx((nv_data *) e));
		if (NV_NEQ(d, e) && 
				nv_entry_is_flag((nv_entry *)d, NEF_DATA_TYPE)) {
			_gc((nv_entry *)d);
		}
		
		#if defined GC_DEBUG
		printf("free data2: %d %s\n", NV_IDX(e), nv_data_str((nv_data *)e, buf, sizeof(buf)));
		#endif

		nv_mm_return_entry(e);
	} else {
		#if defined GC_DEBUG
		printf("free idx1: %d %s\n", NV_IDX(e), nv_idx_str((nv_idx *)e, buf, sizeof(buf)));
		#endif

		nv_idx *i = (nv_idx *) e;
		nv_idx_set_key_idx(i, INVALID_IDX);
		if (nv_entry_is_flag(e, NEF_PERSIST)) {
			nv_entry_unset_flag(e, NEF_PERSIST);
			if (!nv_entry_is_flag(e, NEF_DATA)) { //If it has no data
				_gc(NV_ENTRY(nv_idx_get_data_idx(i)));
				if (INVALID_IDX != nv_idx_get_key_idx(i)) {
					_gc(NV_ENTRY(nv_idx_get_key_idx(i)));
				}
			}
			nv_gc_process(i);
			
			#if defined GC_DEBUG
			printf("free idx2: %d %s\n", NV_IDX(e), nv_idx_str((nv_idx *)e, buf, sizeof(buf)));
			#endif

			nv_mm_return_entry(e);
		}
	}
}

void nv_gc_process(nv_idx *i) {
	char buf[100];
	unsigned prev_i = nv_idx_get_prev_key_idx(i);
	nv_entry *prev = NV_ENTRY(prev_i);	

	#if defined GC_DEBUG
	printf("%d %s\n", NV_IDX(i), nv_idx_str(i, buf, sizeof(buf)));
	#endif

	if (NV_EQ(prev, i) || 
			nv_entry_is_flag((nv_entry *) prev, NEF_DATA_TYPE) ||
			!nv_entry_is_flag((nv_entry *) prev, NEF_PERSIST))
		return;
	if (nv_idx_get_key_idx(i) == nv_idx_get_key_idx((nv_idx *) prev)) {
		_gc(prev);
	}
}
