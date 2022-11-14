#ifndef COORDINATEINDEX
#define COORDINATEINDEX
#include "../include/CoordinateIndex.h"
#endif

#include <iostream>

#define BUCKETSIZE 5

// ------------------- CONSTRUCTORS ------------------------

Coordinate::Coordinate(int la, int lo) {
    latitude = la;
    longitude = lo;
}

BucketQuadTree::BucketQuadTree() {};

BucketQuadTree::BucketQuadTree(Node *node) {
    root = node;
    root->eastLongitude = 0;
    root->westLongitude = 0;
    root->northLatitude = 0;
    root->southLatitude = 0;
    root->NW = nullptr;
    root->NE = nullptr;
    root->SW = nullptr;
    root->SE = nullptr;
    root->bucketNode = true;
    numInsertions = 0;
    numOutOfBounds = 0;
};

// --------------- BUCKETQUADTREE FUNCTIONS ----------------

int calculateHeight(Node *node) {
    if (node == nullptr) {
        return 0;
    }
    int nw_height, ne_height, se_height, sw_height;
    nw_height = calculateHeight(node->NW);
    ne_height = calculateHeight(node->NE);
    sw_height = calculateHeight(node->SW);
    se_height = calculateHeight(node->SE);
    int temp1, temp2, max_height;
    temp1 = std::max(nw_height, ne_height);
    temp2 = std::max(sw_height, se_height);
    max_height = std::max(temp1, temp2);
    return max_height + 1;
}

bool BucketQuadTree::isBucketFull(Node* node) {
    return (node->bucket.size() == BUCKETSIZE);
}

bool BucketQuadTree::isBucketNode(Node *node) {
    return node->bucketNode;
}

bool BucketQuadTree::inBounds(Node* node, Coordinate coords) {
    return (coords.latitude <= node->northLatitude
         && coords.latitude >= node->southLatitude
         && coords.longitude >= node->westLongitude
         && coords.longitude <= node->eastLongitude);
}

void BucketQuadTree::setBoundaries(Node* node, int w, int e, int n, int s) {
    node->westLongitude = w;
    node->eastLongitude = e;
    node->southLatitude = s;
    node->northLatitude = n;
}

void BucketQuadTree::setRootBoundaries(int w, int e, int n, int s) {
    root->westLongitude = w;
    root->eastLongitude = e;
    root->southLatitude = s;
    root->northLatitude = n;
}

void BucketQuadTree::insert(Node* node, Coordinate coordinates, int offset) {

    // If the coordinate isn't in bounds,
    // then it cannot be placed in this tree.
    if (!inBounds(node, coordinates)) {
        numOutOfBounds += 1;
        return;
    }

    if (isBucketNode(node)) {
        // If this is a bucket node we can potentially insert it here.
        // Must check if the bucket is full.
        if (!isBucketFull(node)) {
            node->bucket.push_back(std::make_pair(coordinates, offset));
            numInsertions += 1;
        } else {
            numInsertions -= 5;
            node->bucketNode = false;
            // Calculate new quadrants.
            node->NW = new Node();
            node->SW = new Node();
            node->SE = new Node();
            node->NE = new Node();
            setBoundaries( // Done
                node->NW,
                node->westLongitude,
                (node->eastLongitude - (node->eastLongitude-node->westLongitude)/2),
                node->northLatitude,
                (node->southLatitude + (node->northLatitude - node->southLatitude)/2) 
            );
            setBoundaries(
                node->NE,
                (node->westLongitude + (node->eastLongitude - node->westLongitude)/2),
                node->eastLongitude,
                node->northLatitude,
                (node->southLatitude + (node->northLatitude - node->southLatitude)/2)
            );
            setBoundaries( // Done
                node->SW,
                node->westLongitude,
                (node->eastLongitude - (node->eastLongitude - node->westLongitude)/2),
                (node->northLatitude - (node->northLatitude - node->southLatitude)/2),
                node->southLatitude
            );
            setBoundaries(
                node->SE,
                (node->westLongitude + (node->eastLongitude - node->westLongitude)/2),
                node->eastLongitude,
                (node->northLatitude - (node->northLatitude - node->southLatitude)/2),
                node->southLatitude
            );
            node->bucket.push_back(std::make_pair(coordinates, offset));
            for (auto pair : node->bucket) {
                if (inBounds(node->NW, pair.first)) {
                    insert(node->NW, pair.first, pair.second);
                    continue;
                }
                if (inBounds(node->SW, pair.first)) {
                    insert(node->SW, pair.first, pair.second);
                    continue;
                }
                if (inBounds(node->NE, pair.first)) {
                    insert(node->NE, pair.first, pair.second);
                    continue;
                }
                if (inBounds(node->SE, pair.first)) {
                    insert(node->SE, pair.first, pair.second);
                    continue;
                }
            }
            node->bucket.clear();
        }
    } else {
        // We need to traverse the tree.
        // Possibly calculate which subquadrant it is in.
        if (inBounds(node->NW, coordinates)) {
            insert(node->NW, coordinates, offset);
            return;
        }
        if (inBounds(node->SW, coordinates)) {
            insert(node->SW, coordinates, offset);
            return;
        }
        if (inBounds(node->NE, coordinates)) {
            insert(node->NE, coordinates, offset);
            return;
        }
        if (inBounds(node->SE, coordinates)) {
            insert(node->SE, coordinates, offset);
            return;
        }
    }
};

std::vector<int> BucketQuadTree::search(Node* node, Coordinate coord) {
    std::vector<int> offsets;

    return offsets;
};

bool BucketQuadTree::empty() {
    return root == nullptr;
};

std::string BucketQuadTree::treeToString(Node* node, int rootHeight) {
    std::string output = "";
    int height = calculateHeight(node);
    if (height == rootHeight) {
        // Printing the root node.
        output += (node->bucketNode ? "!" : "@");        
    }
    treeToString(node->NW, rootHeight);
    treeToString(node->NE, rootHeight);
    treeToString(node->SW, rootHeight);
    treeToString(node->SE, rootHeight);
    return output;
}

std::string BucketQuadTree::debug() {
    std::string output = "";
    int height = calculateHeight(root);
    output += "Number of Elements in Quad Tree: " + std::to_string(numInsertions) + "\n"; 
    output += "Number of Elements out of bounds: " + std::to_string(numOutOfBounds) + "\n";
    output += "Height of the Quad Tree: " + std::to_string(height) + "\n";
    output += "Legend:\n";
    output += "@ = non-bucket node\n";
    output += "! = bucket node\n";
    output += "-------------------------------------------------------------------------\n";
    // output += treeToString(root, height) + "\n";
    output += "-------------------------------------------------------------------------\n";
    return output;
}

