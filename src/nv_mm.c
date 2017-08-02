#include "nv_idx.h"
#include "nv_map.h"
#include "nv_mm.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <algorithm>
#include <vector>

struct _nv_driver nv_driver;

static std::deque<nv_entry *> used;
static std::deque<nv_entry *> unused;
static void *mm_start;
static size_t mm_size;
 
void *nv_mm_get_addr() {
	return mm_start;
}

void nv_mm_init(int count) {
	nv_entry *e = (nv_entry *)mm_start;
	int i;
	for (i = 0; i < count; ++i, ++e) {
		char buf[100];
		if (i == 0) continue;

		if (nv_entry_is_flag(e, NEF_PERSIST)) {
			if (!nv_entry_is_flag(e, NEF_DATA_TYPE)) {
				LOG_DEBUG("%3d (%p):%s", NV_IDX(e), e, nv_idx_str((nv_idx *) e, buf, sizeof(buf)));
			} else {
				LOG_DEBUG("%3d (%p):%s", NV_IDX(e), e, nv_data_str((nv_data *) e, buf, sizeof(buf)));
			}
			used.push_back(e);
		} else { 
			
			if (!nv_entry_is_flag(e, NEF_DATA_TYPE)) {
				LOG_DEBUG("\t\tunused %3d (%p):%s", NV_IDX(e), e, nv_idx_str((nv_idx *) e, buf, sizeof(buf)));
			} else {
				LOG_DEBUG("\t\tunused %3d (%p):%s", NV_IDX(e), e, nv_data_str((nv_data *) e, buf, sizeof(buf)));
			}
			
			//LOG_DEBUG("%3d (%p):unused", NV_IDX(e), e);
			unused.push_back(e);
		}
	}
}

std::deque<nv_entry *> *nv_mm_get_used() {
	return &used;
}

int nv_mm_get_unused_count() {
	return unused.size();
}

nv_entry *nv_mm_get_entry(MM_TYPE type) {
	if (unused.empty())
		return NULL;
	nv_entry* e = unused.front();	
	unused.pop_front();
	LOG_DEBUG("%ld left", unused.size());
	//printf("nv_mm_get_entry %d ", NV_IDX(e));
	//fflush(stdout);
	return e;
}

void nv_mm_return_entry(nv_entry *e) {
	memset(e, 0, sizeof(nv_entry));
	//nv_entry_unset_flag(e, NEF_PERSIST | NEF_CONSISTENT);
	mb();
	unused.push_back(e);
}

unsigned nv_entry_get_index(nv_entry *e) {
	intptr_t iptr = (intptr_t) mm_start;
	intptr_t eptr = (intptr_t) e;
	return (eptr - iptr) / sizeof(nv_entry);
}

nv_entry *nv_entry_get_entry(unsigned index) {
	nv_entry *e = (nv_entry *) mm_start;
	e += index;
	return e;
}

void nv_mm_mmap(const char* path, long size, int entry_count) {
	mm_start = nv_driver.mmap(path, size);
	mm_size = size;
	nv_mm_init(entry_count);
}

void nv_mm_unmap() {
	nv_driver.munmap(mm_start, mm_size);	
}

void nv_mm_flush(void *m, size_t size) {
	nv_driver.flush(m, size);
}

void nv_mm_commit() {
	nv_driver.commit();
}

void nv_mm_flush_commit(void *m, size_t size) {
	nv_mm_flush(m, size);
	nv_mm_commit();
}

