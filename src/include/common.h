#ifndef _NV_COMMON_H
#define _NV_COMMON_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//#define LOG_INFO(M, ...) fprintf(stderr, "[INFO][%s(%d):%s] " M "\n", __FILE__, __LINE__, __FUNCTION__,  ##__VA_ARGS__)
//#define LOG_ERR(M, ...) fprintf(stderr, "[ERROR][%s(%d):%s] " M "\n", __FILE__, __LINE__, __FUNCTION__,  ##__VA_ARGS__)
//#define LOG_DEBUG(M, ...) fprintf(stderr, "[DEBUG][%s] " M "\n", __FUNCTION__,  ##__VA_ARGS__)
//#define LOG_INFO(M, ...) fprintf(stderr, "[INFO][%s] " M "\n", __FUNCTION__,  ##__VA_ARGS__)
#define LOG_ERR(M, ...) fprintf(stderr, "[ERROR][%s] " M "\n", __FUNCTION__,  ##__VA_ARGS__)
#define LOG_DEBUG(M, ...)
#define LOG_INFO(M, ...)
//#define LOG_ERR(M, ...)

char *int2bin(int a, char *buffer, int buf_size);

inline void *nv_calulate_ptr(void *a, unsigned offset) {
	return (void *)(((intptr_t) a) + offset);
}

const char* repeat_str(const char* data, char *buf, int length);

int get_random_num(int limit);

const char* get_random_str(char *buf, size_t size);

const char* generate_random_str(char *buf, size_t size);

#endif
