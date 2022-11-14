#include <string>
#include <vector>

class Coordinate {
    public:
        int latitude;
        int longitude;
        Coordinate(int latitude, int longitude);
};

struct Node {
    Node(): bucketNode(true), NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr) {};
    bool bucketNode;
    int eastLongitude; // Expressed in seconds.
    int westLongitude; // Expressed in seconds.
    int northLatitude; // Expressed in seconds.
    int southLatitude; // Expressed in seconds.
    std::vector<std::pair<Coordinate, int>> bucket;
    Node* NW;
    Node* NE;
    Node* SW;
    Node* SE;
};

class BucketQuadTree {
    private:
    public:
        Node* root;
        int nodeBucketSize;
        int numInsertions;
        int numOutOfBounds;
        BucketQuadTree(Node* node);
        BucketQuadTree();
        void setRootBoundaries(int, int, int, int);
        void insert(Node* node, Coordinate coords, int offset);
        std::vector<int> search(Node *node, Coordinate coords);
        void erase(Node *node, Coordinate coords, int offset);
        bool empty();
        bool inBounds(Node* node, Coordinate coordinate);
        bool isBucketFull(Node* node);
        bool isBucketNode(Node *node);
        void makeQuadrants(Node* node);
        void setBoundaries(Node *node, int w, int e, int n, int s);
        std::string treeToString(Node *node, std::string prefix);
        std::string debug();
};

