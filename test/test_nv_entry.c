#include <gtest/gtest.h>

//extern "C" {
	#include "nv_mm.h"
//}

TEST(nv_entry, nv_entry_is_flag_1) {
	nv_entry e;
	nv_entry_set_flag(&e, NEF_PERSIST);
	EXPECT_EQ(true, nv_entry_is_flag(&e, NEF_PERSIST));
}

TEST(nv_entry, nv_entry_is_flag_2) {
	nv_entry e;
	EXPECT_EQ(false, nv_entry_is_flag(&e, NEF_PERSIST));
}

TEST(nv_entry, nv_entry_is_flag_3) {
	nv_entry e;
	nv_entry_set_flag(&e, NEF_PERSIST);
	nv_entry_unset_flag(&e, NEF_PERSIST);
	EXPECT_EQ(false, nv_entry_is_flag(&e, NEF_PERSIST));
}

