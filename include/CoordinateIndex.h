#include <string>
#include <vector>

class Coordinate {
    public:
        int latitude;
        int longitude;
        Coordinate(int latitude, int longitude);
};

struct Node {
    Node(): bucketNode(true), maxPartitions(false), NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr) {};
    bool bucketNode;
    bool maxPartitions;
    int eastLongitude; // Expressed in seconds.
    int westLongitude; // Expressed in seconds.
    int northLatitude; // Expressed in seconds.
    int southLatitude; // Expressed in seconds.
    std::vector<std::pair<Coordinate, std::vector<int>>> bucket;
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
        std::vector<int> searchRegion(Node* node, Coordinate coordinate, int halfHeight, int halfWidth);
        void erase(Node *node, Coordinate coords, int offset);
        bool empty();
        bool inBounds(Node* node, Coordinate coordinate);
        bool inBoundsRegion(Coordinate topRight, Coordinate topLeft, Coordinate botRight, Coordinate botLeft, Coordinate coordinate);
        bool isBucketFull(Node* node);
        bool isBucketNode(Node *node);
        bool checkIfOverlap(Node* node, Coordinate topRight, Coordinate topLeft, Coordinate botRight, Coordinate botLeft);
        void makeQuadrants(Node* node);
        void setBoundaries(Node *node, int w, int e, int n, int s);
        std::string treeToString(Node *node, std::string prefix);
        std::string debug();
};

