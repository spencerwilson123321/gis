#ifndef COORDINATEINDEX
#define COORDINATEINDEX
#include "../include/CoordinateIndex.h"
#endif

// ------------------- CONSTRUCTORS ------------------------

QuadTreeNode::QuadTreeNode(int eastLat, int westLat, int northLong, int southLong, int K) {
    bucketSize = K;
    eastLatitude = eastLat;
    westLatitude = westLat;
    northLongitude = northLong;
    southLongitude = southLong;
    bucketNode = true;
    bucket.resize(bucketSize);
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
}

BucketQuadTree::BucketQuadTree() {
    root = nullptr;
}

BucketQuadTree::BucketQuadTree(QuadTreeNode* node) {
    root = node;
    nodeBucketSize = node->bucketSize;
};

BucketQuadTree::BucketQuadTree(QuadTreeNode* node, int size) {
    root = node;
    nodeBucketSize = size;
};

// --------------- BUCKETQUADTREE FUNCTIONS ----------------

void BucketQuadTree::insert(int latitude, int longitude, int offset) {

};

void BucketQuadTree::search(int latitude, int longitude) {

};

void BucketQuadTree::erase(int latitude, int longitude, int offset) {

};

bool BucketQuadTree::empty() {};

