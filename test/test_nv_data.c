#include <gtest/gtest.h>

//extern "C" {
	#include "nv_data.h"
//}

TEST(nv_data, set_next_idx_1) {
	nv_data d;
	nv_data_set_next_idx(&d, 127);
	char buf[100];
	EXPECT_EQ(127, nv_data_get_next_idx(&d));
}

TEST(nv_data, set_next_idx_2) {
	nv_data d;
	nv_data_set_next_idx(&d, 128);
	char buf[100];
	EXPECT_EQ(128, nv_data_get_next_idx(&d));
}
