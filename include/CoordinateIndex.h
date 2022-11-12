#include <string>
#include <vector>


class QuadTreeNode {
    public:
        bool bucketNode;
        int bucketSize;
        int eastLatitude; // Expressed in seconds.
        int westLatitude; // Expressed in seconds.
        int northLongitude; // Expressed in seconds.
        int southLongitude; // Expressed in seconds.
        std::vector<int> bucket;
        QuadTreeNode* NW;
        QuadTreeNode* NE;
        QuadTreeNode* SW;
        QuadTreeNode* SE;
        QuadTreeNode(int eastLat, int westLat, int northLong, int southLong, int bucketSize);
};


class BucketQuadTree {
    private:
        QuadTreeNode* root;
        int nodeBucketSize;
    public:
    BucketQuadTree();
    BucketQuadTree(QuadTreeNode* root);
    BucketQuadTree(QuadTreeNode* root, int size);
    void insert(int latitude, int longitude, int offset);
    void search(int latitude, int longitude);
    void erase(int latitude, int longitude, int offset);
    bool empty();
};

