#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

#include <algorithm>
#include <map>
#include <string>
#include <set>
#include <vector>

//extern "C" {
	#include "nv_backend_nvdimm.h"
	#include "nv_backend_fs.h"
	#include "nv_map.h"
	#include "nv_mm.h"
	#include "nv_idx.h"
//}
#include "common_config.h"

static std::map<std::string, std::string> _map;

int main() {
	int rc = 0, i;
	nv_idx_data in_data, *out_data; //Important! out_data should be freed!!
	char fmtbuf[30];
	char data[792], key[60];
	nv_map nvm;
	
	long total_size = 0;
	long total_data_size = 0;
	long total_key_length = 0;

	std::vector<std::string> keys;
	std::set<std::string> removed_keys;
	std::set<std::string> unique_keys;
	
	//set seed to generate random numbers
	srand(time(NULL));

	//const unsigned long int total_count = 1000000;
	const unsigned long int total_count = 1000000;

	std::map<std::string, std::string>::iterator it;
	std::set<std::string>::iterator rm_it;
	std::set<std::string>::iterator uk_it;

	INIT_BACKEND();

	nv_mm_mmap("/tmp/mmap_test", mm_size, entry_count);
	nv_map_init(&nvm);
	printf("start a test\n");

	for (i = 0; i < total_count; ++i) {	
		generate_random_str(key, sizeof(key));
		generate_random_str(data, sizeof(data));
		unique_keys.insert(key);

		//printf("%s\t%s\n", key, data);
		printf("%d\tkey: [%s]\n", i, key);

		//if (true) {
		if (((i+1) % 9) != 0) {
			nv_idx_init_data(&in_data, (void *)data, strlen((char *)data));	
			rc = nv_map_put(&nvm, key, &in_data);
			if (rc < 0) {
				if (rc == -ENOSPC) 
					fprintf(stderr, "nv_map_put error: no space\n");
				else
					fprintf(stderr, "nv_map_put error\n");
				goto mm_exit;
			}
			_map[key] = data;
			keys.push_back(key);
			std::set<std::string>::iterator _it = removed_keys.find(key);
			if (_it != removed_keys.end())
				removed_keys.erase(_it);
		} else {
			std::string rm_key = keys[get_random_num(keys.size())];	
			rc = nv_map_remove(&nvm, rm_key.c_str());
			if (rc < 0) {
				fprintf(stderr, "nv_map_remove error\n");
				goto mm_exit;
			}

			std::map<std::string, std::string>::iterator _it = _map.find(rm_key);
			if (_it != _map.end()) {
				_map.erase(_it);
				removed_keys.insert(rm_key);
				printf("removing %s\n", rm_key.c_str());
			}

		}
		
	}

	//Verify the added entries
	for(it = _map.begin(); it != _map.end(); it++) {
		std::string k = it->first;
		std::string v = it->second;

		total_data_size += v.length();
			
		if((out_data = nv_map_get(&nvm, k.c_str())) == NULL) {
			fprintf(stderr, "nv_map_get error\n");
			goto mm_exit;
		}	

		if (strncmp((const char*)out_data->data, v.c_str(), out_data->size) != 0) {
			fprintf(stderr, "wrong value for %s\n", k.c_str());
			if (out_data->size > 0) {
				fprintf(stderr, "\treturned: %s\n", out_data->data);
				fprintf(stderr, "\texpected: %s\n", v.c_str());
			} else
				fprintf(stderr, "0 size value\n");
			nv_idx_free_data(out_data);
			//goto mm_exit;
		} else {
			//fprintf(stdout, "valid value for %s\n", k.c_str());
			nv_idx_free_data(out_data);
		}
	}

	//Verify the removed entries
	for (rm_it = removed_keys.begin(); rm_it != removed_keys.end(); rm_it++) {
		if((out_data = nv_map_get(&nvm, rm_it->c_str())) == NULL) {
			fprintf(stderr, "nv_map_get error\n");
			goto mm_exit;
		}	

		if (out_data != NULL && out_data->size != 0) {
			fprintf(stderr, "invalid value. it should be null for %s\n", rm_it->c_str());
			goto mm_exit;
		}

		if (out_data != NULL) nv_idx_free_data(out_data);
	}

	for(uk_it = unique_keys.begin(); uk_it != unique_keys.end(); uk_it++) {
		total_key_length += uk_it->length();
	}

	printf("\nSuccess!\nmap size: %d\n", nv_map_size(&nvm));
	printf("c++ map size: %d\n", _map.size());
	printf("unique keys: %d\n", unique_keys.size());
	printf("removed keys: %d\n", removed_keys.size());
	printf("used by calculation(MB): %f\n", (total_data_size + total_key_length) / 1024.0 / 1024.0);
	printf("used in nvdimm(MB): %f\n", ((entry_count - nv_mm_get_unused_count()) * 64) / 1024.0 / 1024.0);

	printf("average key size(B): %f\n", total_key_length / (float) unique_keys.size());
	printf("average data size(B): %f\n", total_data_size / (float)_map.size());

	
mm_exit:
	nv_mm_unmap();
	return 0;
}
