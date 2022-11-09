#include "../include/BufferPool.h"


BufferPool::BufferPool() {

};


BufferPool::BufferPool(DatabaseManager& mgr) {
    BufferPool::dbmgr = mgr; 
};

bool BufferPool::isCached() {
    return false; 
}

