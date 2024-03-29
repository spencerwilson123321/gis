#include <vector>
#include <string>
#include <list>
#ifndef GISRECORD
#define GISRECORD
#include "../include/GISRecord.h"
#endif


class CacheEntry {
    public:
        int offset;
        std::string entry;
        CacheEntry(int o, std::string e) {
            offset = o;
            entry = e;
        }
        CacheEntry() {
        }
        bool isEmpty() {
            if (entry == "") {
                return true;
            }
            return false;
        }
};


class BufferPool {
    private:
        int numEntries;
        std::list<CacheEntry> cache;
        std::string dbpath;
    public:
        BufferPool();
        BufferPool(std::string);
        std::string checkCache(int offset);
        void addEntryToCache(CacheEntry entry);
        void updateCache(CacheEntry entry);
        GISRecord retrieveRecord(int offset);
        std::string readFromDatabase(int offset);
        std::string debugPool();
};

