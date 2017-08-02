#include <gtest/gtest.h>

//extern "C" {
	#include "common.h"
//}

TEST(common, int2bin_len_32) {
	char buf[33];
	buf[32] = '\0';
	int2bin(1, buf, sizeof(buf)-1);
	EXPECT_STREQ("00000000000000000000000000000001", buf);
}

TEST(common, int2bin_len_8_1) {
	int i;
	char buf[9];
	buf[8] = '\0';
	int2bin(1, buf, sizeof(buf)-1);
	EXPECT_STREQ("00000001", buf);
}

TEST(common, int2bin_len_8_2) {
	int i;
	char buf[9];
	buf[8] = '\0';
	int2bin(255, buf, sizeof(buf)-1);
	EXPECT_STREQ("11111111", buf);
}

TEST(common, int2bin_len_8_3) {
	int i;
	char buf[9];
	buf[8] = '\0';
	int2bin(256, buf, sizeof(buf)-1);
	EXPECT_STREQ("00000000", buf);
}

TEST(common, int2bin_len_8_4) {
	int i;
	char buf[9];
	buf[8] = '\0';
	int2bin(0, buf, sizeof(buf)-1);
	EXPECT_STREQ("00000000", buf);
}

TEST(common, repeat_str_20) {
	char buf[200];
	const char *result = repeat_str("data", buf, 20);
	buf[20] = '\0';
	EXPECT_EQ(20, strlen(result));
}
