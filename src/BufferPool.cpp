#define BUFFERPOOL 1
#include "../include/BufferPool.h"

#define CACHE_SIZE 16
#include <iostream>

BufferPool::BufferPool() {
    BufferPool::dbpath = "";
};

BufferPool::BufferPool(std::string dbpath) {
    BufferPool::dbpath = dbpath;
};

void BufferPool::updateCache() {
    // This will insert the given CacheEntry object at
    // the front of the cache.
}

std::string BufferPool::checkCache(int offset) {
    // Iterate over the cache and see if it is in here.
    std::list<CacheEntry>::iterator it;
    for (it = BufferPool::cache.begin(); it != BufferPool::cache.end(); ++it) {
        if (it->offset == offset) {
            std::cout << "It's in the cache!" << std::endl;
        }
    }
    return "";
};


GISRecord BufferPool::retrieveRecord(int offset) {
    std::string entry = BufferPool::checkCache(offset);
    if (entry != "") {
        return GISRecord(entry);
    }
    return GISRecord("");
};

