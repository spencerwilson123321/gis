#include <vector>
#include <string>

#ifndef GISRECORD
#include "../include/GISRecord.h"
#endif

#ifndef DBMGR
#include "../include/DatabaseManager.h"
#endif

class BufferPool {
    private:
        DatabaseManager dbmgr;
        std::vector<std::string> cache;
    public:
        BufferPool();
        BufferPool(DatabaseManager& dbmgr);
        bool isCached();
};

