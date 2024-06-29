#include <gtest/gtest.h>

#include "../ac_str_builder.h"

TEST(ACStrBuilderInitTest, InitString) {
    ACStrBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    EXPECT_EQ(sb->str[0], '\0');
    EXPECT_EQ(sb->length, 0);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_free(&sb);
}

TEST(ACStrBuilderInitTest, FreeEmptyString) {
    ACStrBuilder *sb = ac_sb_init();
    ASSERT_NE(sb, nullptr);

    ac_sb_free(&sb);
    EXPECT_EQ(sb, nullptr);
}

class ACStrBuilderTest : public ::testing::Test {
   protected:
    ACStrBuilder *sb;

    void SetUp() override {
        sb = ac_sb_init();
        ASSERT_NE(sb, nullptr);
        ASSERT_EQ(sb->length, 0);
        ASSERT_EQ(sb->_capacity, 20);
    }

    void TearDown() override { free(sb); }
};

TEST_F(ACStrBuilderTest, ResizeDefaultArray) {
    ASSERT_TRUE(_resize_sb_arr(sb, sb->_capacity * 2));
    EXPECT_GE(sb->_capacity, 40);
}

TEST_F(ACStrBuilderTest, Resize40CapArray) {
    sb->_capacity = 40;
    ASSERT_TRUE(_resize_sb_arr(sb, sb->_capacity * 2));
    EXPECT_GE(sb->_capacity, 80);
}

TEST_F(ACStrBuilderTest, AppendCharToEmptyBuilder) {
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);
    EXPECT_STREQ(sb->str, "H");
}

TEST_F(ACStrBuilderTest, AppendCharToBuilderWithSpareCapacity) {
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);

    ac_sb_append_char(sb, 'i');
    EXPECT_EQ(sb->length, 2);
    EXPECT_STREQ(sb->str, "Hi");
}

TEST_F(ACStrBuilderTest, AppendCharToBuilderWithoutSpareCapacity) {
    for (int i = 0; i < 20; ++i) {
        ac_sb_append_char(sb, 'A' + i);
    }
    EXPECT_EQ(sb->length, 20);
    EXPECT_EQ(sb->_capacity, 20);

    ac_sb_append_char(sb, 'Z');
    EXPECT_EQ(sb->length, 21);
    EXPECT_EQ(sb->_capacity, 40);
    EXPECT_STREQ(sb->str, "ABCDEFGHIJKLMNOPQRSTZ");
}

TEST_F(ACStrBuilderTest, TestToString) {
    ac_sb_append_char(sb, 'H');
    ac_sb_append_char(sb, 'i');

    const char *str = ac_sb_to_string(sb);
    ASSERT_STREQ(str, "Hi");
}

TEST_F(ACStrBuilderTest, TestGetCharAt) {
    ac_sb_append_char(sb, 'H');
    ac_sb_append_char(sb, 'i');

    EXPECT_EQ(ac_sb_get_char_at(sb, 0), 'H');
    EXPECT_EQ(ac_sb_get_char_at(sb, 1), 'i');
    EXPECT_EQ(ac_sb_get_char_at(sb, 2), '\0');
}

TEST_F(ACStrBuilderTest, TestGetLength) {
    ac_sb_append_char(sb, 'H');
    ac_sb_append_char(sb, 'i');

    EXPECT_EQ(ac_sb_length(sb), 2);
}

TEST_F(ACStrBuilderTest, AppendValidStringToEmptyStringBuilder) {
    ac_sb_append_str(sb, "Hello, ");
    ac_sb_append_str(sb, "world!");

    EXPECT_EQ(sb->length, 13);
    EXPECT_EQ(sb->_capacity, 20);
    EXPECT_STREQ(sb->str, "Hello, world!");
}

TEST_F(ACStrBuilderTest, AppendValidStringToBuilderWithSpareCapacity) {
    ac_sb_append_char(sb, 'H');
    EXPECT_EQ(sb->length, 1);

    ac_sb_append_str(sb, "ello, world!");

    EXPECT_EQ(sb->length, 13);
    EXPECT_EQ(sb->_capacity, 20);
    EXPECT_STREQ(sb->str, "Hello, world!");
}

TEST_F(ACStrBuilderTest, HandleAppendNullPointer) {
    ac_sb_append_str(sb, NULL);

    EXPECT_EQ(sb->length, 0);
    EXPECT_STREQ(sb->str, "");
}

TEST_F(ACStrBuilderTest, AppendOverflowingValidStringToEmptyStringBuilder) {
    const char *long_str =
        "This is a very long string that will exceed the initial capacity.";
    ac_sb_append_str(sb, long_str);

    EXPECT_EQ(sb->_capacity, 80);
    EXPECT_EQ(sb->length, strlen(long_str));
    EXPECT_STREQ(sb->str, long_str);
}

TEST_F(ACStrBuilderTest, AppendOverflowingValidStringToBuilderWithoutCapacity) {
    ac_sb_append_char(sb, 'T');
    EXPECT_EQ(sb->length, 1);

    const char *long_str =
        "his is a very long string that will exceed the initial capacity.";
    ac_sb_append_str(sb, long_str);

    const char *expected_str =
        "This is a very long string that will exceed the initial capacity.";

    EXPECT_EQ(sb->_capacity, 80);
    EXPECT_EQ(sb->length, strlen(expected_str));
    EXPECT_STREQ(sb->str, expected_str);
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

