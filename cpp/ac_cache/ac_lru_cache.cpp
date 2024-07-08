#include "ac_lru_cache.h"

ACLRUCache::ListNode::ListNode(int k, int v)
    : key(k), val(v), next(nullptr), prev(nullptr) {}
ACLRUCache::ListNode::~ListNode() {}

ACLRUCache::ACLRUCache(size_t capacity)
    : cap(capacity), head(nullptr), tail(nullptr) {}
ACLRUCache::~ACLRUCache() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
}

int ACLRUCache::get(int key) {
    auto it = cache.find(key);
    if (it == cache.end()) {
        return -1;
    }

    ListNode* node = it->second;

    if (node->key != key) {
        return -1;
    }

    if (node == head) {
        return node->val;
    }

    if (node == tail) {
        tail = tail->next;
        tail->prev = nullptr;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->prev = head;
    node->next = nullptr;
    head->next = node;
    head = node;

    return node->val;
}

void ACLRUCache::put(int k, int v) {
    ListNode* new_node = new ListNode(k, v);

    if (!head) {
        head = new_node;
    } else {
        head->next = new_node;
        if (!tail) {
            tail = head;
        }
        head = new_node;
    }

    cache[k] = new_node;
}
