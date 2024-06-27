#include <gtest/gtest.h>

#include "../ac_str_builder.h"

TEST(ACStringBuilderInitTest, InitString) {
    ACStringBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    EXPECT_EQ(sb->str_arr[0], '\0');
    EXPECT_EQ(sb->length, 0);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_free(&sb);
}

TEST(ACStringBuilderInitTest, FreeEmptyString) {
    ACStringBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    ac_sb_free(&sb);
    EXPECT_EQ(sb, nullptr);
}

class ACStringBuilderTest : public ::testing::Test {
   protected:
    ACStringBuilder *sb;

    void SetUp() override {
        sb = ac_sb_init();
        ASSERT_NE(sb, nullptr);
    }

    void TearDown() override { ac_sb_free(&sb); }
};

TEST_F(ACStringBuilderTest, ResizeDefaultArray) {
    ASSERT_TRUE(_resize_sb_arr(sb, sb->_capacity*2));
    EXPECT_GE(sb->_capacity, 40);
}

TEST_F(ACStringBuilderTest, Resize40CapArray) {
    sb->_capacity = 40;
    ASSERT_TRUE(_resize_sb_arr(sb, sb->_capacity*2));
    EXPECT_GE(sb->_capacity, 80);
}

TEST_F(ACStringBuilderTest, AppendCharToBuilderWithSpareCapacity) {
    EXPECT_EQ(sb->length, 0);
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);
    EXPECT_STREQ(sb->str_arr, "H");
}

TEST_F(ACStringBuilderTest, AppendCharToBuilderWithoutSpareCapacity) {
    for (int i = 0; i < 19; ++i) {
        ac_sb_append_char(sb, 'A' + i);
    }
    EXPECT_EQ(sb->length, 19);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_append_char(sb, 'T');
    EXPECT_EQ(sb->length, 20);
    EXPECT_EQ(sb->_capacity, 40);
    EXPECT_STREQ(sb->str_arr, "ABCDEFGHIJKLMNOPQRST");
}

TEST_F(ACStringBuilderTest, TestToString) {
    ac_sb_append_char(sb, 'H');
    ac_sb_append_char(sb, 'i');

    const char *str = ac_sb_to_string(sb);
    ASSERT_STREQ(str, "Hi");
}

TEST_F(ACStringBuilderTest, TestGetCharAt) {
    ac_sb_append_char(sb, 'H');
    ac_sb_append_char(sb, 'i');

    EXPECT_EQ(ac_sb_get_char_at(sb, 0), 'H');
    EXPECT_EQ(ac_sb_get_char_at(sb, 1), 'i');
    EXPECT_EQ(ac_sb_get_char_at(sb, 2), '\0');
}

TEST_F(ACStringBuilderTest, TestLength) {
    ac_sb_append_char(sb, 'H');
    ac_sb_append_char(sb, 'i');

    EXPECT_EQ(ac_sb_length(sb), 2);
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

