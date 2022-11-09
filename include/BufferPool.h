#include <vector>
#include <string>
#include <list>

class BufferPool {
    private:
        std::list<std::string> cache;
        std::string dbpath;
    public:
        BufferPool();
        BufferPool(std::string);
        bool isCached(std::string);
        std::string read();
        bool write();
        std::string debugPool();
};
