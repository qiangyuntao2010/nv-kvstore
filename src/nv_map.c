#include <unistd.h>

#include "nv_data.h"
#include "nv_gc.h"
#include "nv_map.h"

//unsigned long long int tt1 = 0;
//unsigned long long int tt2 = 0;
//unsigned long long int tt3 = 0;
//unsigned long long int tt4 = 0;

int nv_map_init(nv_map *nvm) {
	std::deque<nv_entry *> *used = nv_mm_get_used();
	std::deque<nv_entry *>::iterator it;
	for(it = used->begin(); it != used->end(); ++it) {
		nv_entry *e = *it;	
		if (!nv_entry_is_flag(e, NEF_DATA_TYPE)) {
			nv_idx *i = (nv_idx *) e;	
			
			nv_data *key_e = (nv_data *) NV_ENTRY(nv_idx_get_key_idx(i));
			const char *key = key_e->data;
			std::map<std::string, nv_idx *>::iterator it = nvm->map.find(key);
			if (it != nvm->map.end()) {
				//TODO
				//GC	
			}
			nv_entry_set_flag(e, NEF_CONSISTENT);
			nvm->map[key] = i;
		}
	}
	return 0;
}

inline nv_idx_data *nv_map_init_value(nv_idx_data *value, 
		void *data, size_t size) {
	value->data = data;
	value->size = size;
	return value;
}

bool _has_cycle(nv_idx *n, nv_idx *o) {
	//printf("_has_cycle n: %d, o: %d\n", NV_IDX(n), NV_IDX(o));
	if (o == NULL) {
		return false;
	}

	if (NV_EQ(n, o)) {
		return true;
	} 

	if (nv_entry_is_flag((nv_entry *)o, NEF_DATA_TYPE)) {
		return false;	
	}
	
	if (nv_idx_get_prev_key_idx(o) == 0) {
		return false;
	}
	//printf("<-- %d\n", nv_idx_get_prev_key_idx(o));
	nv_entry* p = NV_ENTRY(nv_idx_get_prev_key_idx(o));
	//char buf[100];
	//printf("%s\n", nv_idx_str((nv_idx *) o, buf, sizeof(buf)));
	if (NV_EQ(p, o)) { //The current node is a last element.
		return false;
	} else {
		return _has_cycle(n, (nv_idx *)p);
	}
}

//struct timespec tps, tpe;

int nv_map_put(nv_map *map, const char *key, nv_idx_data *value) {
	LOG_INFO("key: %s, value_size: %ld", key, value->size); 
	printf("key: %s, value_size: %ld\n", key, value->size); 

	int rc = 0;
	register short flag = 0;

	//clock_gettime(CLOCK_REALTIME, &tps);

	nv_idx* i = (nv_idx *) nv_mm_get_entry(MM_INDEX);
	if (i == NULL) {
		LOG_ERR("no space on the nvdimm");
		return -ENOSPC;
	}
	flag = i->flag;

	//clock_gettime(CLOCK_REALTIME, &tpe);
	//tt1 += (tpe.tv_nsec-tps.tv_nsec);

	//clock_gettime(CLOCK_REALTIME, &tps);

	std::map<std::string, nv_idx *>::iterator it = map->map.find(key);
	if (it != map->map.end()) {
		nv_idx *prev = it->second;
		while(_has_cycle(i, prev)) {
			//printf("!! has cycle %d\n", NV_IDX(i));
			nv_mm_return_entry((nv_entry *)i);
			i = (nv_idx *) nv_mm_get_entry(MM_INDEX);
		}
		flag |= NEF_OP_UPDATE;
		nv_idx *found = it->second;
		nv_idx_set_key_idx(i, nv_idx_get_key_idx(found));
		//nv_idx_set_key_idx(found, INVALID_IDX);
		nv_idx_set_prev_key_idx(i, NV_IDX(found));	
	} else {
		flag |= NEF_OP_ADD;
		nv_data_set_key(i, key, strlen(key));
		nv_idx_set_prev_key_idx(i, NV_IDX(i));	
	}
	nv_idx_set_size(i, value->size);

	//clock_gettime(CLOCK_REALTIME, &tpe);
	//tt2 += (tpe.tv_nsec-tps.tv_nsec);
	
	//clock_gettime(CLOCK_REALTIME, &tps);

	//Store data
	if (value->size <= IDX_BYTES + DATA_BYTES) {
		flag |= NEF_DATA;
		if ((rc = nv_idx_set_data(i, value->data, value->size, false)) != 0) {
			LOG_ERR("nv_idx_set_data failed");
			return rc;
		}
	} else {
		flag &= ~NEF_DATA;
		if ((rc = nv_data_set_data(i, value)) != 0) {
			LOG_ERR("nv_data_set_data failed");
			return rc;
		}
	}

	nv_entry_change_flag((nv_entry *)i, flag);
	map->map[key] = i;
		
	//nv_mm_flush(i, sizeof(nv_idx));

	flag |= NEF_PERSIST;
	nv_entry_change_flag((nv_entry *)i, flag);
	nv_mm_flush(i, sizeof(nv_idx));

	flag |= NEF_CONSISTENT;
	nv_entry_change_flag((nv_entry *)i, flag);
	mb();

	//clock_gettime(CLOCK_REALTIME, &tpe);
	//tt3 += (tpe.tv_nsec-tps.tv_nsec);
	//printf("%d\n", (tpe.tv_nsec-tps.tv_nsec)>>10);
	
	//clock_gettime(CLOCK_REALTIME, &tps);

	nv_gc_process(i);

	//clock_gettime(CLOCK_REALTIME, &tpe);
	//tt4 += (tpe.tv_nsec-tps.tv_nsec);
	//printf("%d\n", (tpe.tv_nsec-tps.tv_nsec));
	return rc;
}

nv_idx_data *nv_map_get(nv_map *map, const char *key) {
	char buf[100];
	nv_idx *i = map->map[key];
	if (i == NULL) {
		LOG_INFO("key: %s, no value", key); 
		return NULL;
	} else {
		LOG_INFO("idx: %s", nv_idx_str(i, buf, sizeof(buf)));
	}

	if (nv_entry_is_flag((nv_entry *)i, NEF_CONSISTENT)) {
		return nv_data_get_data_from_nv_idx(i);
	} else {
		unsigned prev = nv_idx_get_prev_key_idx(i);
		i = (nv_idx *) nv_entry_get_entry(prev);
		return nv_data_get_data_from_nv_idx(i);
	}
}

int nv_map_remove(nv_map *map, const char *key) {
	int rc;
	nv_idx_data in_data;
	nv_idx_init_data(&in_data, (void *)NULL, 0);
	rc = nv_map_put(map, key, &in_data);
	if (rc < 0) {
		LOG_ERR("nv_map_remove failed");	
	}
	return rc;
}

