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

    void TearDown() override {
        free(sl->str_arr);
        free(sl);
    }
};

TEST_F(ACStrListTest, AppendOneElementToEmptyList) {
    const char *string = "Hello, World!";
    char *string_ptr1 = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr1, nullptr);
    strcpy(string_ptr1, string);
    ac_str_list_append(sl, &string_ptr1);

    EXPECT_EQ(sl->length, 1);
    EXPECT_STREQ(sl->str_arr[0], "Hello, World!");

    free(string_ptr1);
}

TEST_F(ACStrListTest, AppendOneElementToListAtCapacity) {
    sl->length = 20;

    const char *string = "Hello, World!";
    char *string_ptr2 = (char *)malloc(strlen(string) + 1);
    ASSERT_NE(string_ptr2, nullptr);
    strcpy(string_ptr2, string);
    ac_str_list_append(sl, &string_ptr2);

    EXPECT_EQ(sl->length, 21);
    EXPECT_EQ(sl->_capacity, 40);
    EXPECT_STREQ(sl->str_arr[20], "Hello, World!");

    free(string_ptr2);
}

TEST_F(ACStrListTest, ReplaceElement) {
    const char *string = "Hello, World!";
    char *string_ptr5 = (char *)malloc(strlen(string) + 1);
    strcpy(string_ptr5, string);
    ac_str_list_append(sl, &string_ptr5);

    const char *string2 = "Goodbye, World!";
    char *string2_ptr5 = (char *)malloc(strlen(string2) + 1);
    strcpy(string2_ptr5, string2);
    ac_str_list_append(sl, &string2_ptr5);

    const char *string3 = "Hello again, World!";
    char *string3_ptr5 = (char *)malloc(strlen(string3) + 1);
    strcpy(string3_ptr5, string3);
    ac_str_list_replace(sl, &string3_ptr5, 2);  // out-of-bounds

    EXPECT_EQ(sl->length, 2);

    ac_str_list_replace(sl, &string3_ptr5, 1);
    EXPECT_STREQ(sl->str_arr[1], "Hello again, World!");

    free(string_ptr5);
    free(string2_ptr5);
    free(string3_ptr5);
}

TEST_F(ACStrListTest, TestGetStringAt) {
    const char *string = "Hello, World!";
    char *string_ptr6 = (char *)malloc(strlen(string) + 1);
    strcpy(string_ptr6, string);
    ac_str_list_append(sl, &string_ptr6);

    const char *string2 = "Goodbye, World!";
    char *string2_ptr6 = (char *)malloc(strlen(string2) + 1);
    strcpy(string2_ptr6, string2);
    ac_str_list_append(sl, &string2_ptr6);

    EXPECT_STREQ(ac_str_list_get_str_at(sl, 0), "Hello, World!");
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 1), "Goodbye, World!");

    free(string_ptr6);
    free(string2_ptr6);
}

TEST_F(ACStrListTest, TestRemoveStringAt) {
    const char *string = "Hello, World!";
    char *string_ptr7 = (char *)malloc(strlen(string) + 1);
    strcpy(string_ptr7, string);
    ac_str_list_append(sl, &string_ptr7);

    const char *string2 = "Goodbye, World!";
    char *string2_ptr7 = (char *)malloc(strlen(string2) + 1);
    strcpy(string2_ptr7, string2);
    ac_str_list_append(sl, &string2_ptr7);

    ac_str_list_remove(sl, 0);

    EXPECT_EQ(sl->length, 1);
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 0), "Goodbye, World!");
    EXPECT_EQ(ac_str_list_get_str_at(sl, 1), nullptr);

    free(string2_ptr7);
}

TEST_F(ACStrListTest, TestRemoveStringAtInOversizedArray) {
    const char *string = "Hello, World!";
    char *string_ptr8 = (char *)malloc(strlen(string) + 1);
    strcpy(string_ptr8, string);
    ac_str_list_append(sl, &string_ptr8);

    const char *string2 = "Goodbye, World!";
    char *string2_ptr8 = (char *)malloc(strlen(string2) + 1);
    strcpy(string2_ptr8, string2);
    ac_str_list_append(sl, &string2_ptr8);

    sl->_capacity = 40;
    ASSERT_EQ(sl->_capacity, 40);
    ASSERT_EQ(sl->length, 2);
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 1), "Goodbye, World!");

    ac_str_list_remove(sl, 0);

    EXPECT_EQ(sl->length, 1);
    ASSERT_EQ(sl->_capacity, 20);
    EXPECT_STREQ(ac_str_list_get_str_at(sl, 0), "Goodbye, World!");
    EXPECT_EQ(ac_str_list_get_str_at(sl, 1), nullptr);

    free(string2_ptr8);
}

TEST_F(ACStrListTest, TestGetLength) {
    const char *string = "Hello, World!";
    char *string_ptr9 = (char *)malloc(strlen(string) + 1);
    strcpy(string_ptr9, string);
    ac_str_list_append(sl, &string_ptr9);

    const char *string2 = "Goodbye, World!";
    char *string2_ptr9 = (char *)malloc(strlen(string2) + 1);
    strcpy(string2_ptr9, string2);
    ac_str_list_append(sl, &string2_ptr9);

    EXPECT_EQ(ac_str_list_length(sl), 2);

    free(string_ptr9);
    free(string2_ptr9);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
