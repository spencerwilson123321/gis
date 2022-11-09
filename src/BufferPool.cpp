#define BUFFERPOOL 1
#include "../include/BufferPool.h"

BufferPool::BufferPool() {

};

BufferPool::BufferPool(std::string dbpath) {
    BufferPool::dbpath = dbpath;
};

bool BufferPool::isCached(std::string entry) {
    return false;
};

std::string BufferPool::read() {
    return "";
};

bool BufferPool::write() {
    return false;
};
