#define INIT_BACKEND init_nvdimm_backend

static long mm_size = 1024L * 1024 * 1024;
//static long mm_size = 16L * 1024 * 1024 * 1024;
static int entry_count = mm_size / 64;
//static int entry_count = 30;

