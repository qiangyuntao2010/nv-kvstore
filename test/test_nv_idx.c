#include <gtest/gtest.h>

//extern "C" {
	#include "nv_idx.h"
//}

TEST(nv_idx, set_key_idx) {
	nv_idx i;
	unsigned prev_key_idx = 1;
	nv_idx_set_prev_key_idx(&i, prev_key_idx);
	EXPECT_EQ(prev_key_idx, nv_idx_get_prev_key_idx(&i));
}

TEST(nv_idx, set_valid_key) {
	nv_idx i;
	const char* name = "1234567890123456789012345";
	EXPECT_EQ(0, nv_idx_set_key(&i, name));
}

TEST(nv_idx, set_invalid_key) {
	nv_idx i;
	const char* name = "12345678901234567890123456";
	EXPECT_EQ(EINVAL, nv_idx_set_key(&i, name));
}

TEST(nv_idx, set_null_key) {
	nv_idx i;
	EXPECT_EQ(EINVAL, nv_idx_set_key(&i, NULL));
}

TEST(nv_idx, set_size_1) {
	nv_idx i;
	nv_idx_set_size(&i, 128);
	char buf[100];
	EXPECT_EQ(128, nv_idx_get_size(&i));
}

TEST(nv_idx, set_size_2) {
	nv_idx i;
	nv_idx_set_size(&i, 16777215);
	char buf[100];
	EXPECT_EQ(16777215, nv_idx_get_size(&i));
}
