#include <gtest/gtest.h>

#include "../ac_str_list.h"

TEST(StringListTest, InitArrayWithCapacityOf20) {
    ACStrList *sl = ac_str_list_init(20);
    ASSERT_NE(sl, nullptr);
    EXPECT_EQ(sl->length, 0);
    EXPECT_EQ(sl->_capacity, 20);
    ac_str_list_free(&sl);
}

TEST(StringListTest, InitArrayWith0Capacity) {
    ACStrList *sl = ac_str_list_init(0);
    ASSERT_NE(sl, nullptr);
    EXPECT_EQ(sl->length, 0);
    EXPECT_EQ(sl->_capacity, 20);
    ac_str_list_free(&sl);
}

TEST(StringListTest, FreeEmptyStringList) {
    ACStrList *sl = ac_str_list_init(20);
    ASSERT_NE(sl, nullptr);
    ac_str_list_free(&sl);
    EXPECT_EQ(sl, nullptr);
}

class ACStrListTest : public ::testing::Test {
   protected:
    ACStrList *sl;

    void SetUp() override {
        sl = ac_str_list_init(0);
        ASSERT_NE(sl, nullptr);
        ASSERT_EQ(sl->length, 0);
        ASSERT_EQ(sl->_capacity, 20);
    }

    void TearDown() override { ac_str_list_free(&sl); }
};

TEST_F(ACStrListTest, AppendOneElementToEmptyList) {
    const char *string = "Hello, World!";
    char *string_ptr = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr, nullptr);
    strcpy(string_ptr, string);
    ac_str_list_append(sl, &string_ptr);

    EXPECT_EQ(sl->length, 1);
    EXPECT_STREQ(sl->str_arr[0], "Hello, World!");
}

TEST_F(ACStrListTest, AppendOneElementToListAtCapacity) {
    ACStrList *sl2 = ac_str_list_init(0);
    ASSERT_NE(sl2, nullptr);
    ASSERT_EQ(sl2->length, 0);
    ASSERT_EQ(sl2->_capacity, 20);
    sl2->length = 20;
    ASSERT_EQ(sl2->length, 20);

    const char *string = "Hello, World!";
    char *string_ptr = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr, nullptr);
    strcpy(string_ptr, string);
    ac_str_list_append(sl2, &string_ptr);

    EXPECT_EQ(sl2->length, 21);
    EXPECT_EQ(sl2->_capacity, 40);
    EXPECT_STREQ(sl2->str_arr[20], "Hello, World!");
}

TEST_F(ACStrListTest, TestGetStringAt) {
    const char *string = "Hello, World!";
    char *string_ptr = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr, nullptr);
    strcpy(string_ptr, string);
    ac_str_list_append(sl, &string_ptr);

    const char *string2 = "Goodbye, World!";
    char *string2_ptr = (char *)malloc(strlen(string2) + 1);
    ASSERT_NE(string2_ptr, nullptr);
    strcpy(string2_ptr, string2);
    ac_str_list_append(sl, &string2_ptr);

    EXPECT_STREQ(ac_str_list_get_str_at(sl, 0), "Hello, World!");
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 1), "Goodbye, World!");
}

TEST_F(ACStrListTest, TestRemoveStringAt) {
    const char *string = "Hello, World!";
    char *string_ptr = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr, nullptr);
    strcpy(string_ptr, string);
    ac_str_list_append(sl, &string_ptr);

    const char *string2 = "How is it going, World!";
    char *string2_ptr = (char *)malloc(strlen(string2) + 1);
    ASSERT_NE(string2_ptr, nullptr);
    strcpy(string2_ptr, string2);
    ac_str_list_append(sl, &string2_ptr);

    const char *string3 = "Goodbye, World!";
    char *string3_ptr = (char *)malloc(strlen(string2) + 1);
    ASSERT_NE(string3_ptr, nullptr);
    strcpy(string3_ptr, string3);
    ac_str_list_append(sl, &string3_ptr);

    ASSERT_EQ(sl->length, 3);
    ASSERT_STREQ(ac_str_list_get_str_at(sl, 2), "Goodbye, World!");

    ac_str_list_remove(sl, 1);

    EXPECT_EQ(sl->length, 2);
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 0), "Hello, World!");
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 1), "Goodbye, World!");
    EXPECT_EQ(ac_str_list_get_str_at(sl, 2), nullptr);
}

TEST_F(ACStrListTest, TestRemoveStringAtInOversizedArray) {
    sl->_capacity = 40;

    const char *string = "Hello, World!";
    char *string_ptr = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr, nullptr);
    strcpy(string_ptr, string);
    ac_str_list_append(sl, &string_ptr);

    const char *string2 = "How is it going, World!";
    char *string2_ptr = (char *)malloc(strlen(string2) + 1);
    ASSERT_NE(string2_ptr, nullptr);
    strcpy(string2_ptr, string2);
    ac_str_list_append(sl, &string2_ptr);

    const char *string3 = "Goodbye, World!";
    char *string3_ptr = (char *)malloc(strlen(string2) + 1);
    ASSERT_NE(string3_ptr, nullptr);
    strcpy(string3_ptr, string3);
    ac_str_list_append(sl, &string3_ptr);

    ASSERT_EQ(sl->length, 3);
    ASSERT_EQ(sl->_capacity, 40);
    ASSERT_STREQ(ac_str_list_get_str_at(sl, 2), "Goodbye, World!");

    ac_str_list_remove(sl, 1);

    EXPECT_EQ(sl->length, 2);
    ASSERT_EQ(sl->_capacity, 20);
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 0), "Hello, World!");
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 1), "Goodbye, World!");
    EXPECT_EQ(ac_str_list_get_str_at(sl, 2), nullptr);
}

TEST_F(ACStrListTest, TestGetLength) {
    const char *string = "Hello, World!";
    char *string_ptr = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr, nullptr);
    strcpy(string_ptr, string);
    ac_str_list_append(sl, &string_ptr);

    EXPECT_EQ(ac_str_list_length(sl), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
