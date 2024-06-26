struct Entry {
    char *key;
    char *value;
    struct Entry *next;
};

struct HashTable {
    struct Entry **buckets;
    int size;
};

