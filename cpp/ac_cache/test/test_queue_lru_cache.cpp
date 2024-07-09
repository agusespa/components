#include <algorithm>
#include <vector>

class VectorLRUCache {
   private:
    int capacity;
    std::vector<std::pair<int, int> > cache;

    struct KeyFinder {
        int key;
        KeyFinder(int k) : key(k) {}
        bool operator()(const std::pair<int, int>& element) const {
            return element.first == key;
        }
    };

   public:
    VectorLRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        std::vector<std::pair<int, int> >::iterator it;
        for (it = cache.begin(); it != cache.end(); ++it) {
            if (it->first == key) {
                int value = it->second;
                cache.erase(it);
                cache.push_back(std::make_pair(key, value));
                return value;
            }
        }
        return -1;
    }

    void put(int key, int value) {
        std::vector<std::pair<int, int> >::iterator it;
        it = std::find_if(cache.begin(), cache.end(), KeyFinder(key));

        if (it != cache.end()) {
            cache.erase(it);
        } else if (cache.size() >= capacity) {
            cache.erase(cache.begin());
        }

        cache.push_back(std::make_pair(key, value));
    }
};
