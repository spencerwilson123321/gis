#include <vector>
#include <string>


class BufferPool {
    private:
        std::vector<std::string> cache;
        std::string dbpath;
    public:
        BufferPool();
        BufferPool(std::string dbpath);
        bool isCached();
};

