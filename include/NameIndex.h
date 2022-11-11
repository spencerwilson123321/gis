#include <string>
#include <vector>

class Hashtable{
    enum BucketStatus {EMPTY, OCCUPIED, DELETED};
    int numCollisions;
    std::vector<int> buckets;
    std::vector<BucketStatus> bucketStatus;
public:
    Hashtable(int n);
    Hashtable();
    void initializeTable(int n);
    void debug();
    int getValue(std::string key);
    void insert(std::string key, int value);
    bool search(std::string key);
    bool erase(std::string key);
    int getNumCollisions();
    unsigned int hashFunction(std::string key);
    void collisionFunction(std::string key);
};
