#ifndef _NV_MAP_H
#define _NV_MAP_H

#include "nv_idx.h"
#include "nv_mm.h"

#include <map>
#include <vector>
#include <string>

typedef struct _map {	
	std::map<std::string, nv_idx *> map;
} nv_map;

int nv_map_init(nv_map *nvm);

nv_idx_data *nv_map_init_value(nv_idx_data *value, void *data, size_t size);

int nv_map_put(nv_map *map, const char *key, nv_idx_data *value);

//nv_idx_data *nv_map_get(nv_map *map, const char *key, nv_idx_data *value);
nv_idx_data *nv_map_get(nv_map *map, const char *key);

int nv_map_remove(nv_map *map, const char *key);

inline int nv_map_size(nv_map *map) {
	return map->map.size();	
}

//extern unsigned long long int tt1;
//extern unsigned long long int tt2;
//extern unsigned long long int tt3;
//extern unsigned long long int tt4;

#endif
