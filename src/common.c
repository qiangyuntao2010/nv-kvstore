#include "common.h"
#include "stdio.h"

char *int2bin(int a, char *buffer, int buf_size) {
	int i;
	int last_idx = buf_size - 1;
    buffer += last_idx;
    for (i = last_idx; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';
        a >>= 1;
    }
    return buffer;
}

const char* repeat_str(const char* data, char *buf, int length) {
	size_t len = strlen(data), cur;
	for (cur = 0; cur < length; cur += len) {
		if (cur + len < length) {
			strncpy(buf+cur, data, len);
		} else {
			strncpy(buf+cur, data, length - cur);
		}
	}
	return buf;
}

int get_random_num(int limit) {
	int r = (rand() % limit);
	if (r == 0) ++r;
	return r;
}

const char* generate_random_str(char *buf, size_t size) {
	int r = get_random_num(size);
	if (r < 2) r = 3;
	return get_random_str(buf, r);
}

const char* get_random_str(char *buf, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_!#$%^&*()";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            buf[n] = charset[key];
        }
        buf[size] = '\0';
    }
    return buf;
}
