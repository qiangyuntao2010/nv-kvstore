#ifndef _NV_MM_H
#define _NV_MM_H

#include <sys/mman.h>

#include <list>
#include <deque>

#include "nv_data.h"

#define NV_MM_ALIGN 64

#define NEF_PERSIST 0x01
#define NEF_CONSISTENT 0x02
#define NEF_GC 0x04
#define NEF_DATA 0x08 //If index entry contains data
#define NEF_OP_ADD 0x10
#define NEF_OP_UPDATE 0x20
#define NEF_KEY 0x40
#define NEF_DATA_TYPE 0x80	//index: 0, data: 1

#define mb() asm volatile("mfence" ::: "memory")
#define rmb() asm volatile("lfence" ::: "memory")
#define wmb() asm volatile("sfence" ::: "memory")

#define MM_TYPE bool
#define MM_INDEX false
#define MM_DATA true

#define NV_IDX(e) nv_entry_get_index((nv_entry *) e)
#define NV_ENTRY(i) nv_entry_get_entry(i)
#define NV_EQ(a, b) ((nv_entry *)a == (nv_entry *)b)
#define NV_NEQ(a, b) ((nv_entry *)a != (nv_entry *)b)

typedef struct _entry {
	char flag;
	char data[63];
} __attribute__((packed)) nv_entry;

inline bool nv_entry_is_flag(nv_entry *e, short flag) {
	return e->flag & flag;	
}

inline void nv_entry_set_flag(nv_entry *e, short flag) {
	e->flag |= flag;
}

inline void nv_entry_unset_flag(nv_entry *e, short flag) {
	e->flag &= ~flag;
}

inline void nv_entry_change_flag(nv_entry *e, short flag) {
	e->flag = flag;
}

void nv_mm_init(void *start, int count);

void *nv_mm_get_addr();

nv_entry *nv_mm_get_entry(MM_TYPE type);

void nv_mm_return_entry(nv_entry *e);

std::deque<nv_entry *> *nv_mm_get_used();

int nv_mm_get_unused_count();

unsigned nv_entry_get_index(nv_entry *e);

nv_entry *nv_entry_get_entry(unsigned idx);

void nv_mm_mmap(const char* path, long size, int entry_count);

void nv_mm_unmap();

void nv_mm_flush(void *m, size_t length);

void nv_mm_commit();

void nv_mm_flush_commit(void *m, size_t size);

extern struct _nv_driver {
	void *(*mmap)(const char*, long);
	void (*flush)(void *, size_t);
	void (*commit)();
	void (*munmap)(void *, size_t);
} nv_driver;

#endif
