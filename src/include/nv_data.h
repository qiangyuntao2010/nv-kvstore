#ifndef _NV_DATA_H
#define _NV_DATA_H

#include "common.h"
#include "nv_idx.h"

#define NV_DATA_DATA_BYTES (64-1-IDX_BYTES)

typedef struct _data {
	char flag;
	char next_idx[IDX_BYTES];
	char data[NV_DATA_DATA_BYTES];
} __attribute__((packed)) nv_data;

int nv_data_set_next_idx(nv_data *d, size_t val);

unsigned nv_data_get_next_idx(nv_data *d);

size_t nv_data_store(nv_data *d, nv_idx_data *value, int offset);

nv_idx_data *nv_data_get_data(nv_idx *i);

int nv_data_set_data(nv_idx *i, nv_idx_data *value);
	
int nv_data_set_key(nv_idx *i, const char* key, size_t length);

const char* nv_data_str(nv_data *d, char* buf, size_t length);

nv_idx_data *nv_data_get_data_from_nv_idx(nv_idx *i);

#endif
