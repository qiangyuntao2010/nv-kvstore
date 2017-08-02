#include "nv_backend_fs.h"
#include "nv_mm.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void *nv_fs_map(const char* filepath, long size) {
	int fd;
	void *mm = NULL;

	fd = open(filepath, O_RDWR|O_CREAT);
	if (fd < 0) {
		perror("file open error");
		goto final;
	}

	if (ftruncate(fd, size) < 0) {
		perror("ftruncate error");
		goto fd_release;
	}

	mm = mmap(0, size, O_RDWR|O_CREAT, MAP_SHARED, fd, 0);
	if (mm == MAP_FAILED) {
		perror("mmap error");
		goto fd_release;
	}

fd_release:
	close(fd);
final:
	return mm;
}

void nv_fs_flush(void *m, size_t length) {
	mb();
	msync(m, length, MS_SYNC);
	mb();
}

void nv_fs_pcommit() {
	//Nothing
}

void nv_fs_unmap(void *m, size_t size) {
	munmap(m, size);
}

void init_fs_backend() {
	nv_driver.mmap = nv_fs_map;
	nv_driver.flush = nv_fs_flush;
	nv_driver.commit = nv_fs_pcommit;
	nv_driver.munmap = nv_fs_unmap;
}
