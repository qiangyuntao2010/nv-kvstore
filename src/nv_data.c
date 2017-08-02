#include <unistd.h>

#include "nv_data.h"
#include "nv_mm.h"

int nv_data_init(nv_data *d) {
	memset(d, 0, sizeof(nv_data));	
}

int nv_data_set_next_idx(nv_data *d, size_t val) {
	if (val < IDX_MAX) {
		d->next_idx[2] = val >> 16 & 0xff;
		d->next_idx[1] = val >> 8 & 0xff;
		d->next_idx[0] = val & 0xff;
		return 0;
	}
	return EINVAL;
}

unsigned nv_data_get_next_idx(nv_data *d) {
	return (d->next_idx[0] & 0xFF) | 
			(d->next_idx[1] & 0xFF) << 8 | 
			(d->next_idx[2] & 0xFF) << 16;
}

size_t nv_data_store(nv_data *d, nv_idx_data *value, int offset) {
	char *ptr = (char *) value->data;
	ptr += offset;
	size_t left = value->size - offset;
	if (left <= sizeof(d->data)) {
		memcpy(d->data, (void *)ptr, left);
		return left;
	} else {
		memcpy(d->data, (void *)ptr, sizeof(d->data));
		return sizeof(d->data);
	}
} 

//variable 'additonal' is a size of the addtional space to store left bytes
int nv_data_build(nv_idx_data *value, unsigned additional, 
			short flag, nv_data **result) {
	unsigned sz = value->size - additional;
	unsigned offset = 0;
	int entry_count = sz / NV_DATA_DATA_BYTES;
	int left_bytes = sz % NV_DATA_DATA_BYTES;
	int j, j_end;
	if (left_bytes != 0) entry_count++;

	nv_data *d = (nv_data *) nv_mm_get_entry(MM_DATA);	
	*result = d;
	if (d == NULL) return -ENOSPC;
	nv_entry_set_flag((nv_entry *)d, NEF_DATA_TYPE | NEF_PERSIST | flag);	
	offset += nv_data_store(d, value, offset);

	nv_data *precedent = d;
	//j_end = (additional > 0 && left_bytes != 0) ? entry_count-1 : entry_count;
	//for (j = 0; j < j_end; ++j) {
	for (j = 0; j < (entry_count -1); ++j) {
		d = (nv_data *) nv_mm_get_entry(MM_DATA);	
		if (d == NULL) {
			LOG_ERR("no space");
			return -ENOSPC;
		}
		nv_entry_set_flag((nv_entry *)d, NEF_DATA_TYPE | NEF_PERSIST | flag);	
		offset += nv_data_store(d, value, offset);

		nv_data_set_next_idx(precedent, nv_entry_get_index((nv_entry *)d));
		nv_mm_flush(precedent, sizeof(nv_data));

		precedent = d;
	}

	if (offset >= value->size-1) {
		nv_data_set_next_idx(precedent, nv_entry_get_index((nv_entry *)precedent));
	}
	nv_mm_flush(precedent, sizeof(nv_data));
	return offset;
}

int nv_data_set_data(nv_idx *i, nv_idx_data *value) {
	nv_data *d;
	int offset;
	short flag;
	offset = nv_data_build(value, DATA_BYTES, 0, &d);
	if (offset < 0) {
		return offset;
	} else if (offset < value->size) {
//printf("hahaahaha\n");
		nv_idx_set_data(i, ((char *)value->data)+offset, 
				value->size - offset, true);
	}
	nv_idx_set_data_idx(i, nv_entry_get_index((nv_entry *)d));
	return 0;
}

int nv_data_set_key(nv_idx *i, const char* key, size_t length) {
	nv_data *d;
	int offset;
	short flag;
	nv_idx_data value;
	nv_idx_init_data(&value, (void *)key, length);
	offset = nv_data_build(&value, 0, NEF_KEY, &d);
	if (offset < 0) {
		return offset;
	}
	nv_idx_set_key_idx(i, nv_entry_get_index((nv_entry *)d));
	return 0;
}

nv_idx_data *nv_data_get_data(nv_idx *i) {
	nv_idx_data *value = (nv_idx_data *) malloc(sizeof(nv_idx_data));
	unsigned size = nv_idx_get_size(i);
	void *data = malloc(size);
	value->data = data;
	value->size = size;

	unsigned sz = size - DATA_BYTES;
	int entry_count = sz / NV_DATA_DATA_BYTES;
	int left_bytes = sz % NV_DATA_DATA_BYTES;
	unsigned offset = 0;
	int j;
	char buf[100];
	if (left_bytes != 0) entry_count++;

	unsigned i_idx = nv_entry_get_index((nv_entry *) i);
	unsigned di = nv_idx_get_data_idx(i);
	if (di == i_idx) {
		LOG_ERR("Invalid index: %s\n", 
				nv_idx_str(i, buf, sizeof(buf)));
		return NULL;
	}

	unsigned next_di;
	for (j = 0; j < entry_count; ++j) {
		nv_data *d = (nv_data *) nv_entry_get_entry(di);
		if ((offset + sizeof(d->data) < size)) {
			//printf("start: %p, end: %p\n", data, nv_calulate_ptr(data, size));
			//printf("current: %p\n", nv_calulate_ptr(data, offset));
			//printf("offset: %d\n", offset);
			//printf("d->data: %p\n", d->data);
			memcpy(nv_calulate_ptr(data, offset), d->data, sizeof(d->data)); 
			offset += sizeof(d->data);
		} else {
			memcpy(nv_calulate_ptr(data, offset), d->data, size-offset); 
			offset = size;
		}		
		next_di = nv_data_get_next_idx(d);
		//printf("next di: %u\n", next_di);
		if (di == next_di)
			break;
		di = next_di;
	}

	if (offset < size) {
		unsigned left = size - offset;
		if (left <= DATA_BYTES) {
			offset += nv_idx_copy_data(i, nv_calulate_ptr(data, offset), left, true);
			return value;
		}
	} else if (offset == size) {
		return value;
	}
	LOG_ERR("Wrong index size: %s\n", 
			nv_idx_str(i, buf, sizeof(buf)));
	LOG_ERR("offset: %d, size: %d\n", offset, size); 
	return NULL;
}

const char* nv_data_str(nv_data *d, char* buf, size_t length) {
	char buffer[9];
	buffer[8] = '\0';
	int2bin(d->flag, buffer, sizeof(buffer)-1);
	if (nv_entry_is_flag((nv_entry *)d, NEF_KEY)) {
		snprintf(buf, length, "[flag: %s, next: %d, data: %s, len: %d]",
				buffer, nv_data_get_next_idx(d), 
				(char *)d->data, strlen(d->data));
	} else {
		snprintf(buf, length, "[flag: %s, next: %d]",
				buffer, nv_data_get_next_idx(d));
	}
	return buf;	
}

nv_idx_data *nv_data_get_data_from_nv_idx(nv_idx *i) {
	if (nv_entry_is_flag((nv_entry *)i, NEF_DATA)) {	
		return nv_idx_get_data(i, false); 		
	} else {
		return nv_data_get_data(i);
	}
}
