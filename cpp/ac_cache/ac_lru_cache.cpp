#include "ac_lru_cache.h"

ACLRUCache::ListNode::ListNode(int k, int v)
    : key(k), val(v), next(nullptr), prev(nullptr) {}

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

void ACLRUCache::addToFront(ListNode* node) {
    if (!head) {
        head = node;
    } else {
        node->prev = head;
        node->next = nullptr;
        head->next = node;
        if (!tail) {
            tail = head;
        }
        head = node;
    }
}

void ACLRUCache::removeNode(ListNode* node) {
    if (node == tail) {
        tail = tail->next;
        tail->prev = nullptr;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
}

void ACLRUCache::moveToFront(ListNode* node) {
    if (node == head) return;
    removeNode(node);
    addToFront(node);
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

    moveToFront(node);
    return node->val;
}

void ACLRUCache::put(int k, int v) {
    auto it = cache.find(k);
    if (it != cache.end()) {
        ListNode* node = it->second;
        node->val = v;
        moveToFront(node);
    } else {
        ListNode* newNode = new ListNode(k, v);

        cache[k] = newNode;
        addToFront(newNode);

        if (cache.size() > cap) {
            ListNode* toRemove = tail;
            removeNode(toRemove);
            cache.erase(toRemove->key);
            delete (toRemove);
        }
    }
}
