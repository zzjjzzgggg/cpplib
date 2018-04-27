#include <iostream>
#include <string>

#include "../adv/LRUCache.h"

int main(int argc, char* argv[]) {
    lru::Cache<std::string, std::string> cache(3);
    cache.insert("hello", "world");
    cache.insert("foo", "bar");

    std::cout << "checking refresh : " << cache.get("hello") << std::endl;
    cache.insert("hello1", "world1");
    cache.insert("foo1", "bar1");

    return 0;
}
