#include <stdlib.h>

#include "common.h"
#include "nv_idx.h"

static const char* temp_key = "test";

inline int nv_idx_init(nv_idx *i) {
	memset(i, 0, sizeof(nv_idx));	
}
/*
int nv_idx_set_prev_key_idx(nv_idx *i, unsigned val) {
	if (val < KEY_IDX_MAX) {
		i->prev_key_idx[2] = val >> 16 & 0xff;
		i->prev_key_idx[1] = val >> 8 & 0xff;
		i->prev_key_idx[0] = val & 0xff;
		return 0;
	}
	return EINVAL;
}

unsigned nv_idx_get_prev_key_idx(nv_idx *i) {
	return (i->prev_key_idx[0] & 0xFF) | 
			(i->prev_key_idx[1] & 0xFF) << 8 | 
			(i->prev_key_idx[2] & 0xFF) << 16;
}
*/

int nv_idx_set_data_idx(nv_idx *i, unsigned val) {
	if (val < IDX_MAX) {
		i->data_idx[3] = val >> 24 & 0xff;
		i->data_idx[2] = val >> 16 & 0xff;
		i->data_idx[1] = val >> 8 & 0xff;
		i->data_idx[0] = val & 0xff;
		return 0;
	}
	return EINVAL;
}

unsigned nv_idx_get_data_idx(nv_idx *i) {
    return (i->data_idx[0] & 0xFF) | 
			(i->data_idx[1] & 0xFF) << 8 | 
			(i->data_idx[2] & 0xFF) << 16 |
			(i->data_idx[3] & 0xFF) << 24;
}

unsigned nv_idx_get_size(nv_idx *i) {
    return (i->size[0] & 0xFF) | 
			(i->size[1] & 0xFF) << 8 | 
			(i->size[2] & 0xFF) << 16;
}

int nv_idx_set_size(nv_idx *i, size_t size) {
	if (size < SIZE_MAX) {
		i->size[2] = size >> 16 & 0xff;
		i->size[1] = size >> 8 & 0xff;
		i->size[0] = size & 0xff;
		return 0;
	}
	return EINVAL;
}

inline const char* nv_idx_get_key(nv_idx *i) {
	//return i->key;
	return temp_key;
}

int nv_idx_set_key(nv_idx *i, const char* name) {
	/*
	if (name != NULL && strlen(name) <= sizeof(i->key)) {
		memcpy(i->key, name, strlen(name));
		return 0;
	}
	return EINVAL;
	*/
	return 0;
}

const char* nv_idx_str(nv_idx *i, char* buf, size_t length) {
	char buffer[9];
	buffer[8] = '\0';
	int2bin(i->flag, buffer, sizeof(buffer)-1);
	snprintf(buf, length, 
			"[key: %d, back-ref: %d, size: %u, flag: %s, data idx: %d]",
			nv_idx_get_key_idx(i), nv_idx_get_prev_key_idx(i), nv_idx_get_size(i), 
			buffer, nv_idx_get_data_idx(i));
	return buf;	
}

int nv_idx_set_data(nv_idx *i, void *data, size_t size, bool keep_di) {
	//printf(">>> nv_idx_set_data: %s\n", (char *)data);
	if (size == 0) 	//If it is null
		return 0;
	if (keep_di) {
		if (size <= DATA_BYTES) {
			memcpy(i->data, data, size);
			return 0;
		}
	} else {
		if (size <= IDX_BYTES + DATA_BYTES) {
			memcpy(i->data_idx, data, size);
			return 0;
		}
	}
	return EINVAL;
}

nv_idx_data *nv_idx_get_data(nv_idx *i, bool keep_di) {
	nv_idx_data *value = (nv_idx_data *) malloc(sizeof(nv_idx_data));
	unsigned size = nv_idx_get_size(i);
	value->size = size;
	if (size > 0) {
		void *data = malloc(size);
		value->data = data;
		if (keep_di)
			memcpy(data, i->data, size);
		else
			memcpy(data, i->data_idx, size);
	} else {
		value->data = NULL;
	}
	return value;
}

unsigned nv_idx_copy_data(nv_idx *i, void *target, unsigned size, bool keep_di) {
	//unsigned size = nv_idx_get_size(i);	
	if (keep_di)
		memcpy(target, i->data, size);
	else
		memcpy(target, i->data_idx, size);
	return size;	
}

nv_idx_data *nv_idx_init_data(nv_idx_data *value, void *data, size_t size) {
	value->data = data;
	value->size = size;
	return value;
}

void nv_idx_free_data(nv_idx_data *value) {
	if (value != NULL) {
		if (value->data != NULL) free(value->data);
		free(value);
	}
}

