#include <gtest/gtest.h>

#include <chrono>
#include <random>
#include <iostream>

/* #define RUN_LONG_TESTS */

#include "../ac_lru_cache.h"
#include "test_queue_lru_cache.cpp"

TEST(ACLRUCacheTest, ConstructorTest) {
    ACLRUCache cache(2);
    EXPECT_EQ(cache.get(1), -1);  // No element inserted yet, should return -1
}

TEST(ACLRUCacheTest, BasicPutGetTest) {
    ACLRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.get(1), 1);   // returns 1
    EXPECT_EQ(cache.get(2), 2);   // returns 2
    EXPECT_EQ(cache.get(3), -1);  // returns -1 (not found)
}

TEST(ACLRUCacheTest, BasicUpdateGetTest) {
    ACLRUCache cache(2);
    EXPECT_EQ(cache.get(2), -1);  // returns -1 (not found)
    cache.put(2, 6);
    EXPECT_EQ(cache.get(1), -1);  // returns -1 (not found)
    cache.put(1, 5);
    cache.put(1, 2);
    EXPECT_EQ(cache.get(1), 2);  // returns 2
    EXPECT_EQ(cache.get(2), 6);  // returns 6
}

TEST(ACLRUCacheTest, PutGetTestSequence) {
    ACLRUCache cache(2);  // Initialize cache with capacity 2

    cache.put(2, 1);             // Put (2, 1)
    cache.put(2, 2);             // Put (2, 2)
    EXPECT_EQ(cache.get(2), 2);  // Get 2 -> should return 2

    cache.put(1, 1);              // Put (1, 1)
    cache.put(4, 1);              // Put (4, 1), evicts key 2 (LRU policy)
    EXPECT_EQ(cache.get(2), -1);  // Get 2 -> should return -1 (not found)
}

TEST(ACLRUCacheTest, LRUEvictionTest) {
    ACLRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.get(1), 1);   // returns 1
    cache.put(3, 3);              // evicts key 2
    EXPECT_EQ(cache.get(2), -1);  // returns -1 (not found)
    cache.put(4, 4);              // evicts key 1
    EXPECT_EQ(cache.get(1), -1);  // returns -1 (not found)
    EXPECT_EQ(cache.get(3), 3);   // returns 3
    EXPECT_EQ(cache.get(4), 4);   // returns 4
}

TEST(ACLRUCacheTest, CapacityTest) {
    ACLRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.get(1), 1);   // returns 1
    cache.put(3, 3);              // evicts key 2
    EXPECT_EQ(cache.get(2), -1);  // returns -1 (not found)
    EXPECT_EQ(cache.get(3), 3);   // returns 3
    cache.put(4, 4);              // evicts key 1
    EXPECT_EQ(cache.get(1), -1);  // returns -1 (not found)
    EXPECT_EQ(cache.get(4), 4);   // returns 4
}

TEST(ACLRUCacheTest, FrequentGetTest) {
    ACLRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.get(1), 1);   // returns 1
    cache.put(3, 3);              // evicts key 2
    EXPECT_EQ(cache.get(2), -1);  // returns -1 (not found)
    EXPECT_EQ(cache.get(3), 3);   // returns 3
    EXPECT_EQ(cache.get(1), 1);   // returns 1, key 1 should not be evicted
    cache.put(4, 4);              // evicts key 3
    EXPECT_EQ(cache.get(3), -1);  // returns -1 (not found)
    EXPECT_EQ(cache.get(1), 1);   // returns 1
    EXPECT_EQ(cache.get(4), 4);   // returns 4
}

TEST(ACLRUCacheTest, UpdateExistingKeyTest) {
    ACLRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(cache.get(1), 1);   // returns 1
    cache.put(1, 10);             // update key 1 with new value 10
    EXPECT_EQ(cache.get(1), 10);  // returns 10
    cache.put(3, 3);              // evicts key 2
    EXPECT_EQ(cache.get(2), -1);  // returns -1 (not found)
    EXPECT_EQ(cache.get(3), 3);   // returns 3
}

#ifdef RUN_LONG_TESTS
TEST(ACLRUCacheTest, PerformanceTest) {
    const int num_operations = 1000000;
    ACLRUCache cache(3000);
    VectorLRUCache queue(3000);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    std::vector<int> keys(num_operations);
    for (int i = 0; i < num_operations; ++i) {
        keys[i] = dis(gen);
    }

    // unoptimized baseline for put operations
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        queue.put(keys[i], i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> queue_duration = end - start;
    std::cout << "put vector based LRU duration: " << queue_duration.count() << std::endl;
    double expected_duration = queue_duration.count() / 30;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        cache.put(keys[i], i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "put optimized LRU duration: " << duration.count() << std::endl;
    ASSERT_LT(duration.count(), expected_duration);

    // unoptimized baseline for get operations
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        queue.get(keys[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    queue_duration = end - start;
    std::cout << "get vector based LRU duration: " << queue_duration.count() << std::endl;
    expected_duration = queue_duration.count() / 200;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_operations; ++i) {
        cache.get(keys[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "get optimized LRU duration: " << duration.count() << std::endl;
    EXPECT_LT(duration.count(), expected_duration);
}
#endif

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
