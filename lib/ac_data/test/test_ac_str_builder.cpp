#include <gtest/gtest.h>

#include "../ac_str_builder.h"

TEST(ACStringBuilderTest, InitString) {
    ACStringBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    EXPECT_EQ(sb->str_arr[0], '\0');
    EXPECT_EQ(sb->length, 0);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_free(&sb);
}

TEST(ACStringBuilderTest, FreeEmptyString) {
    ACStringBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    ac_sb_free(&sb);
    EXPECT_EQ(sb, nullptr);
}

TEST(ACStringBuilderTest, ResizeArray) {
    ACStringBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    ASSERT_TRUE(_resize_sb_arr(sb));
    EXPECT_GE(sb->_capacity, 40);

    ac_sb_free(&sb);
}

TEST(ACStringBuilderTest, AppendCharToBuilderWithSpareCapacity) {
    ACStringBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    EXPECT_EQ(sb->length, 0);
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);
    EXPECT_STREQ(sb->str_arr, "H");

    ac_sb_free(&sb);
}

TEST(ACStringBuilderTest, AppendCharToBuilderWithoutSpareCapacity) {
    ACStringBuilder *sb = ac_sb_init();
    for (int i = 0; i < 19; ++i) {
        ac_sb_append_char(sb, 'A' + i);
    }

    EXPECT_EQ(sb->length, 19);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_append_char(sb, 'T');
    EXPECT_EQ(sb->length, 20);
    EXPECT_EQ(sb->_capacity, 40);
    EXPECT_STREQ(sb->str_arr, "ABCDEFGHIJKLMNOPQRST");

    ac_sb_free(&sb);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
