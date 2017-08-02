#ifndef _NV_IDX_H
#define _NV_IDX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define IDX_BYTES 4
#define SIZE_BYTES 3
#define DATA_BYTES 47
#define IDX_MAX ((long) 1 << (IDX_BYTES * 8))
#define SIZE_MAX ((long) 1 << (SIZE_BYTES * 8))
#define INVALID_IDX 0

typedef struct _idx {
	char flag;
	unsigned int key_idx;
	unsigned int prev_key_idx;
	char size[SIZE_BYTES]; 	//16MB
	char data_idx[IDX_BYTES];
	char data[DATA_BYTES];
} __attribute__((packed)) nv_idx;

typedef struct _idx_data {
	void *data;
	size_t size;
} nv_idx_data;

int nv_idx_init(nv_idx *i);

inline unsigned int nv_idx_get_key_idx(nv_idx *i) {
	return i->key_idx;
}

inline void nv_idx_set_key_idx(nv_idx *i, unsigned int key_idx) {
	i->key_idx = key_idx;
}

inline int nv_idx_set_prev_key_idx(nv_idx *i, unsigned int val) {
	i->prev_key_idx = val;
}

inline unsigned int nv_idx_get_prev_key_idx(nv_idx *i) {
	return i->prev_key_idx;
}

int nv_idx_set_data_idx(nv_idx *i, unsigned val);

unsigned nv_idx_get_data_idx(nv_idx *i);

nv_idx_data *nv_idx_get_data(nv_idx *i, bool keep_di);

unsigned nv_idx_copy_data(nv_idx *i, void *target, unsigned size, bool keep_di);

const char* nv_idx_get_key(nv_idx *i);

int nv_idx_set_key(nv_idx *i, const char* name);

const char* nv_idx_str(nv_idx *i, char* buf, size_t length);

int nv_idx_set_data(nv_idx *i, void *data, size_t size, bool keep_di);

nv_idx_data *nv_idx_get_data(nv_idx *i, nv_idx_data *data, bool keep_di);

nv_idx_data *nv_idx_init_data(nv_idx_data *value, void *data, size_t size);

void nv_idx_free_data(nv_idx_data *value);

unsigned nv_idx_get_size(nv_idx *i);

int nv_idx_set_size(nv_idx *i, size_t size);

#endif
