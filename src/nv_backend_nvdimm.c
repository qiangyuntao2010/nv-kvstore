#include "nv_backend_nvdimm.h"
#include "nv_mm.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "agigaram_lib.h"
#include "agigaram_smbus_interface.h"

#define CL_SIZE 64

static int agigaram_fd;

inline void clflush(volatile void *p)
{
	mb();
    asm volatile("clflush %0" : "+m" (*(volatile char *)p));
	mb();
}

void *nv_nvdimm_map(const char* filepath, long size) {
	AgigaLib_NvDimmInit(true);
	return AgigaLib_GetNvDimmMemAddress();
	//if((agigaram_fd = open("/dev/agigaram",O_RDWR | O_SYNC )) < 0){
		//LOG_ERR("Failed to open /dev/agigaram");
		//return NULL;
	//}
	//return mmap(0, 1024L*1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED ,agigaram_fd, 0); // 3L*1024*1024*1024);
}

void nv_nvdimm_flush(void *m, size_t length) {
	int o;
	for (o = 0; o < length; o += CL_SIZE) {
		clflush(nv_calulate_ptr(m, o));
	}
}

void nv_nvdimm_pcommit() {
	//Nothing
	//It isn't necessary for the device which has a super capicitor.
}

void nv_nvdimm_munmap(void *m, size_t size) {
	AgigaLib_NvDimmDeInit();
	//ioctl(agigaram_fd, IOCTL_SYNC_D_CACHE, NULL);
	//close(agigaram_fd);
}

void init_nvdimm_backend() {
	nv_driver.mmap = nv_nvdimm_map;
	nv_driver.flush = nv_nvdimm_flush;
	nv_driver.commit = nv_nvdimm_pcommit;
	nv_driver.munmap = nv_nvdimm_munmap;
}
