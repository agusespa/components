#include <gtest/gtest.h>

#include "../ac_string.h"

TEST(StringTest, InitString) {
    ACString *str = ac_string_init();
    ASSERT_NE(str, nullptr);
    EXPECT_EQ(str->length, 0);
    EXPECT_EQ(str->_capacity, 10);
    ac_string_free(&str);
}

TEST(StringTest, FreeEmptyString) {
    ACString *str = ac_string_init();
    ASSERT_NE(str, nullptr);
    ac_string_free(&str);
    EXPECT_EQ(str, nullptr);
}

/* TEST(ArrayListTest, AppendOneElementToEmptyList) { */
/*     StrList *list = str_list_init(0); */
/*     ASSERT_NE(list, nullptr); */

/*     double *ptr_double = malloc(sizeof(double)); */
/*     *ptr_double = 3.14; */
/*     str_list_append(list, ptr_double); */

/*     EXPECT_EQ(list->length, 1); */
/*     EXPECT_EQ(list->data[0], 3.14); */
/*     str_list_free(&list); */
/* } */

/* TEST(ArrayListTest, RemoveElement) { */
/*     ArrayList array; */
/*     init_array(&array, 2); */
/*     add_element(&array, 1); */
/*     add_element(&array, 2); */
/*     add_element(&array, 3); */
/*     remove_element(&array, 1);  // Remove element at index 1 */
/*     EXPECT_EQ(array.length, 2); */
/*     EXPECT_EQ(array.data[0], 1); */
/*     EXPECT_EQ(array.data[1], 3); */
/*     free_array(&array); */
/* } */

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
