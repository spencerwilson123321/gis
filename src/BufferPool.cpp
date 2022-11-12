#define BUFFERPOOL 1
#include "../include/BufferPool.h"

#define CACHE_SIZE 16
#include <iostream>
#include <fstream>
#include <limits>

BufferPool::BufferPool() {
    BufferPool::dbpath = "";
    numEntries = 0;
};


BufferPool::BufferPool(std::string dbpath) {
    BufferPool::dbpath = dbpath;
    numEntries = 0;
};


std::string BufferPool::checkCache(int offset) {
    // Iterate over the cache and see if it is in here.
    std::list<CacheEntry>::iterator it;
    std::string entry;
    for (it = BufferPool::cache.begin(); it != BufferPool::cache.end(); ++it) {
        if (it->offset == offset) {
            entry = it->entry;
            auto beg = cache.begin();
            cache.insert(beg, *it);
            cache.erase(it);
            return entry;
        }
    }
    return "";
};


GISRecord BufferPool::retrieveRecord(int offset) {
    std::string entry = BufferPool::checkCache(offset);
    if (entry != "") {
        GISRecord record(entry);
        return record;
    }
    entry = BufferPool::readFromDatabase(offset);
    GISRecord record(entry);
    return record;
};


std::string BufferPool::debugPool() {
    std::string output = "Contents of cache:\n";
    output += "Offset | Record\n";
    output += "MRU\n";
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        output += std::to_string(it->offset) + " | " + it->entry + "\n";
    }
    output += "LRU\n";
    return output;
}


void BufferPool::addEntryToCache(CacheEntry entry) {
    if (CACHE_SIZE == cache.size()) {
        // Cache is full so remove the least recently used element.
        cache.pop_back();
        cache.push_front(entry);
    } else {
        cache.push_front(entry);
        numEntries += 1;
    }
};


std::string BufferPool::readFromDatabase(int offset) {
    std::ifstream db;
    std::string entry;
    db.open(BufferPool::dbpath, std::ios::in);
    if (!db.is_open()) {
        // Open file failed.
        return "";
    }
    db.seekg(db.beg);
    for (int i = 0; i < offset-1; ++i) {
        db.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    std::getline(db, entry);
    db.close();
    CacheEntry cacheEntry(offset, entry);
    addEntryToCache(cacheEntry);
    return entry;
}

