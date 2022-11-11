#include <vector>
#include <string>
#include <list>
#include <map>
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
};


class Node {
    public:
        CacheEntry value;
        Node* next;
        Node* prev;
        Node() {
            value = CacheEntry();
            next = nullptr;
            prev = nullptr;
        }

        Node(CacheEntry value, Node* n, Node *p) {
            value = value;
            next = n;
            prev = p;
        }
};


class BufferPool {
    private:
        std::list<CacheEntry> cache;
        std::string dbpath;
    public:
        BufferPool();
        BufferPool(std::string);
        std::string checkCache(int offset);
        void updateCache();
        GISRecord retrieveRecord(int offset);
        std::string debugPool();
};
