#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "common.h"
#include "agigaram_lib.h"
#include "agigaram_smbus_interface.h"
#include "common_config.h"

int gtg(unsigned char nvdimm_idx);

int main() {
	int dimm_count;
	int i;

	AgigaLib_NvDimmInit(true);
	void *mm = AgigaLib_GetNvDimmMemAddress();

	printf("Clearing %dB\n", mm_size);
	memset(mm, 0, mm_size); 
	AgigaLib_NvDimmDeInit();
	return 0;
}

int gtg(unsigned char nvdimm_idx)
{
	unsigned char value;
	int rc = FALSE;
	char buf[32];

	rc = nvdimm_get_power_mode(nvdimm_idx,&value);
	if(rc != SUCCESS)
		return FALSE;
	int2bin(value, buf, sizeof(buf));
	printf("power_mode: %s\n", buf);

	rc = nvdimm_get_gtg_1(nvdimm_idx,&value);
	if(rc != SUCCESS)
		return FALSE;
	int2bin(value, buf, sizeof(buf));
	printf("gtg: %s\n", buf);

	return rc;
}

