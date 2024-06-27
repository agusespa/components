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
        ASSERT_EQ(sb->length, 0);
        ASSERT_EQ(sb->_capacity, 20);
    }

    void TearDown() override { ac_sb_free(&sb); }
};

TEST_F(ACStringBuilderTest, ResizeDefaultArray) {
    ASSERT_TRUE(_resize_sb_arr(sb, sb->_capacity * 2));
    EXPECT_GE(sb->_capacity, 40);
}

TEST_F(ACStringBuilderTest, Resize40CapArray) {
    sb->_capacity = 40;
    ASSERT_TRUE(_resize_sb_arr(sb, sb->_capacity * 2));
    EXPECT_GE(sb->_capacity, 80);
}

TEST_F(ACStringBuilderTest, AppendCharToEmptyBuilder) {
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);
    EXPECT_STREQ(sb->str_arr, "H");
}

TEST_F(ACStringBuilderTest, AppendCharToBuilderWithSpareCapacity) {
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);

    ac_sb_append_char(sb, 'i');
    EXPECT_EQ(sb->length, 2);
    EXPECT_STREQ(sb->str_arr, "Hi");
}

TEST_F(ACStringBuilderTest, AppendCharToBuilderWithoutSpareCapacity) {
    for (int i = 0; i < 20; ++i) {
        ac_sb_append_char(sb, 'A' + i);
    }
    EXPECT_EQ(sb->length, 20);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_append_char(sb, 'Z');
    EXPECT_EQ(sb->length, 21);
    EXPECT_EQ(sb->_capacity, 40);
    EXPECT_STREQ(sb->str_arr, "ABCDEFGHIJKLMNOPQRSTZ");
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

TEST_F(ACStringBuilderTest, AppendValidStringToEmptyStringBuilder) {
    ac_sb_append_str(sb, "Hello, ");
    ac_sb_append_str(sb, "world!");

    EXPECT_EQ(sb->length, 13);
    EXPECT_EQ(sb->_capacity, 20);
    EXPECT_STREQ(sb->str_arr, "Hello, world!");
}

TEST_F(ACStringBuilderTest, AppendValidStringToBuilderWithSpareCapacity) {
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);

    ac_sb_append_str(sb, "ello, world!");

    EXPECT_EQ(sb->length, 13);
    EXPECT_EQ(sb->_capacity, 20);
    EXPECT_STREQ(sb->str_arr, "Hello, world!");
}

TEST_F(ACStringBuilderTest, HandleAppendNullPointer) {
    ac_sb_append_str(sb, NULL);

    EXPECT_EQ(sb->length, 0);
    EXPECT_STREQ(sb->str_arr, "");
}

TEST_F(ACStringBuilderTest, AppendOverflowingValidStringToEmptyStringBuilder) {
    const char *long_str =
        "This is a very long string that will exceed the initial capacity.";
    ac_sb_append_str(sb, long_str);

    EXPECT_EQ(sb->_capacity, 80);
    EXPECT_EQ(sb->length, strlen(long_str));
    EXPECT_STREQ(sb->str_arr, long_str);
}

TEST_F(ACStringBuilderTest,
       AppendOverflowingValidStringToBuilderWithoutCapacity) {
    ac_sb_append_char(sb, 'T');
    EXPECT_EQ(sb->length, 1);

    const char *long_str =
        "his is a very long string that will exceed the initial capacity.";
    ac_sb_append_str(sb, long_str);

    const char *expected_str =
        "This is a very long string that will exceed the initial capacity.";

    EXPECT_EQ(sb->_capacity, 80);
    EXPECT_EQ(sb->length, strlen(expected_str));
    EXPECT_STREQ(sb->str_arr, expected_str);
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

