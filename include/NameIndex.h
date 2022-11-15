#include <string>
#include <vector>

class Hashtable{
    enum BucketStatus {EMPTY, OCCUPIED, DELETED};
    int numCollisions;
    std::vector<std::pair<std::string, int>> buckets;
    std::vector<BucketStatus> bucketStatus;
    int numOccupied;
    float loadFactor;
public:
    Hashtable(int n);
    Hashtable();
    
    void initializeTable(int n);
    std::string debug();

    bool search(std::string key);
    int get(std::string key);
    void add(std::string key, int value);
    void remove(std::string key);
    void rehash();

    unsigned int hashFunction(std::string key);
    void collisionFunction(std::string key, int value);
};
