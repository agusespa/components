#ifndef AC_LRU_CACHE_H
#define AC_LRU_CACHE_H

#include <cstddef>
#include <unordered_map>

class ACLRUCache {
   public:
    ACLRUCache(size_t capacity);
    ~ACLRUCache();
    int get(const int key);
    void put(const int key, const int value);

    class ListNode {
       public:
        int key;
        int val;
        ListNode* next;
        ListNode* prev;
        ListNode(int key, int value);
        ~ListNode();
    };

   private:
    size_t cap;
    std::unordered_map<int, ListNode*> cache;
    ListNode* head;
    ListNode* tail;

    void addToFront(ListNode* node);
    void removeNode(ListNode* node);
    void moveToFront(ListNode* node);
};

#endif  // AC_LRU_CACHE_H
