#define BUFFERPOOL 1
#include "../include/BufferPool.h"

BufferPool::BufferPool() {

};

BufferPool::BufferPool(std::string dbpath) {
    BufferPool::dbpath = dbpath; 
};

bool BufferPool::isCached() {
    return false; 
}

