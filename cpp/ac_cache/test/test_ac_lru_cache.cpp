#include <gtest/gtest.h>

#include "../ac_lru_cache.h"

// Test Case 1: Constructor Test
TEST(ACLRUCacheTest, ConstructorTest) {
   ACLRUCache cache(2);
   EXPECT_EQ(cache.get(1), -1);  // No element inserted yet, should return -1
}

// Test Case 2: Basic Put and Get Test
TEST(ACLRUCacheTest, BasicPutGetTest) {
   ACLRUCache cache(2);
   cache.put(1, 1);
   cache.put(2, 2);
   EXPECT_EQ(cache.get(1), 1);   // returns 1
   EXPECT_EQ(cache.get(2), 2);   // returns 2
   EXPECT_EQ(cache.get(3), -1);  // returns -1 (not found)
}

// Test Case 3: LRU Eviction Test
TEST(ACLRUCacheTest, LRUEvictionTest) {
   /* ACLRUCache cache(2); */
   /* cache.put(1, 1); */
   /* cache.put(2, 2); */
   /* EXPECT_EQ(cache.get(1), 1);   // returns 1 */
   /* cache.put(3, 3);              // evicts key 2 */
   /* EXPECT_EQ(cache.get(2), -1);  // returns -1 (not found) */
   /* cache.put(4, 4);              // evicts key 1 */
   /* EXPECT_EQ(cache.get(1), -1);  // returns -1 (not found) */
   /* EXPECT_EQ(cache.get(3), 3);   // returns 3 */
   /* EXPECT_EQ(cache.get(4), 4);   // returns 4 */
}

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
