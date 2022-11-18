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
    root->maxPartitions = false;
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

int calculateNumElements(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    if (node->bucketNode) {
        int sum = 0;
        for (auto pair : node->bucket) {
            sum += pair.second.size();
        }
        return sum;
    }
    int numElementsNW = calculateNumElements(node->NW);
    int numElementsNE = calculateNumElements(node->NE);
    int numElementsSW = calculateNumElements(node->SW);
    int numElementsSE = calculateNumElements(node->SE);
    return numElementsNE + numElementsNW + numElementsSE + numElementsSW;
}

bool BucketQuadTree::isBucketFull(Node* node) {
    return (node->bucket.size() >= BUCKETSIZE);
}

bool BucketQuadTree::isBucketNode(Node *node) {
    return node->bucketNode;
}

bool BucketQuadTree::inBounds(Node* node, Coordinate coords) {
    bool result = (coords.latitude <= node->northLatitude
         && coords.latitude >= node->southLatitude
         && coords.longitude >= node->westLongitude
         && coords.longitude <= node->eastLongitude);
    return result;
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
        return;
    }

    if (isBucketNode(node)) {
        // Check if the coordinate matches one already in the bucket.
        bool match = false;
        for (int i = 0; i < node->bucket.size(); i++) {
            if (node->bucket[i].first.latitude == coordinates.latitude && node->bucket[i].first.longitude == coordinates.longitude) {
                match = true;
                node->bucket[i].second.push_back(offset);
            }
        }
        // If no match, then we need to create a new coordinate index.
        // But first check that the bucket isn't already full.
        if (!match) {
            if (!isBucketFull(node)) { // If the bucket isn't full, then add the new entry and all is good.
                std::vector<int> offsets;
                offsets.push_back(offset);
                node->bucket.push_back(std::make_pair(coordinates, offsets));
            } else  { // If the bucket is full, then we need to create new subregions and move everything into those subregions.
                node->bucketNode = false;
                node->NW = new Node();
                node->SW = new Node();
                node->SE = new Node();
                node->NE = new Node();
                setBoundaries(
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
                setBoundaries(
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
                // Get all the offsets and coordinates into a list of pairs, then iterate the list of pairs
                // and insert each one into the corresponding subregion.
                // 1. Create a vector of pair<Coordinate, int> for each coordinate and offset.
                std::vector<std::pair<Coordinate, int>> tempVector;
                for (auto pair : node->bucket) {
                    for (int ofst : pair.second) {
                        tempVector.push_back(std::make_pair(pair.first, ofst));
                    }
                }
                tempVector.push_back(std::make_pair(coordinates, offset)); // Make sure we add the current coordinate,offset pair.
                for (auto pair : tempVector) {
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
        }
    } else {
        // We need to traverse the tree.
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
}

std::vector<int> BucketQuadTree::search(Node* node, Coordinate coord) {
    std::vector<int> offsets;
    if (!inBounds(node, coord)) {
        return offsets;
    }
    if (isBucketNode(node)) {
        // Check if there are coordinates matching our search coordinates.
        for (auto pair : node->bucket) {
            if (pair.first.latitude == coord.latitude && pair.first.longitude == coord.longitude) {
                // Coordinates match, add every offset for this coordinate index.
                for (int o : pair.second) {
                    offsets.push_back(o);
                }
            }
        }
        return offsets;
    } else {
        if (inBounds(node->NW, coord)) {
            return search(node->NW, coord);
        }
        if (inBounds(node->NE, coord)) {
            return search(node->NE, coord);
        }
        if (inBounds(node->SW, coord)) {
            return search(node->SW, coord);
        }
        if (inBounds(node->SE, coord)) {
            return search(node->SE, coord);
        }
    }
    return offsets;
};

bool BucketQuadTree::inBoundsRegion(Coordinate topRight, Coordinate topLeft, Coordinate botRight, Coordinate botLeft, Coordinate coordinate) {
    int northLat = topRight.latitude;
    int southLat = botRight.latitude;
    int eastLat = topRight.longitude;
    int westLat = topLeft.longitude;
    return (coordinate.latitude >= southLat
         && coordinate.latitude <= northLat
         && coordinate.longitude >= westLat
         && coordinate.longitude <= eastLat);
};

bool BucketQuadTree::checkIfOverlap(Node* node, Coordinate topRight, Coordinate topLeft, Coordinate botRight, Coordinate botLeft) {
    int northLat = topRight.latitude;
    int southLat = botRight.latitude;
    int eastLong = topRight.longitude;
    int westLong = topLeft.longitude;
    return (node->northLatitude >= southLat
         && node->southLatitude <= northLat
         && node->eastLongitude >= westLong
         && node->westLongitude <= eastLong);
}


std::vector<int> BucketQuadTree::searchRegion(Node* node, Coordinate coord, int halfHeight, int halfWidth) {
    std::vector<int> offsets;
    std::vector<int> result;
    // The four corners of the region search:
    // Idea: If a region search overlaps a subquadrant in the coordinate space then one of it's corners must be within
    // that quadrants bounds.
    Coordinate topLeft(coord.latitude+halfHeight, coord.longitude-halfWidth);
    Coordinate topRight(coord.latitude+halfHeight, coord.longitude+halfWidth);
    Coordinate botLeft(coord.latitude-halfHeight, coord.longitude-halfWidth);
    Coordinate botRight(coord.latitude-halfHeight, coord.longitude+halfWidth);

    if (node == nullptr) {
        return offsets;
    }

    if (!checkIfOverlap(node, topRight, topLeft, botRight, botLeft)) {
        // The region being searched does not overlap with the current node, so return nothing.
        // Nothing is found.
        return offsets;
    }

    if (isBucketNode(node)) {
        // If this is a bucket node, and one of the corners is in this region.
        // Then we need to add all offsets that are within the bounds
        // of the region being searched.
        for (auto pair : node->bucket) {
            if (inBoundsRegion(topRight, topLeft, botRight, botLeft, pair.first)) {
                // offsets.push_back(pair.second);
                for (int o : pair.second) {
                    offsets.push_back(o);
                }
            }
        }
        return offsets;
    } else {
        // Recursively search the tree.
        result = searchRegion(node->NW, coord, halfHeight, halfWidth);
        if (result.size() != 0) {
            offsets.insert(offsets.end(), result.begin(), result.end());
        }
        result = searchRegion(node->NE, coord, halfHeight, halfWidth);
        if (result.size() != 0) {
            offsets.insert(offsets.end(), result.begin(), result.end());
        }
        result = searchRegion(node->SW, coord, halfHeight, halfWidth);
        if (result.size() != 0) {
            offsets.insert(offsets.end(), result.begin(), result.end());
        }
        result = searchRegion(node->SE, coord, halfHeight, halfWidth);
        if (result.size() != 0) {
            offsets.insert(offsets.end(), result.begin(), result.end());
        }
    }
    return offsets;
};


bool BucketQuadTree::empty() {
    return root == nullptr;
};

std::string BucketQuadTree::treeToString(Node* node, std::string prefix) {
    std::string output = "";
    if (node == nullptr) return output;
    std::string value = (node->bucketNode ? "B" : "N");
    output += prefix + value + "\n";
    if (node->bucketNode) {
        for (auto pair : node->bucket) {
            for (auto o : pair.second) {
                output += prefix + " Lat: " + std::to_string(pair.first.latitude) + " Long: " + std::to_string(pair.first.longitude) + " File Offset: " + std::to_string(o) + "\n";
            }
        }
    }
    output += treeToString(node->NW, prefix+"   ");
    output += treeToString(node->NE, prefix+"   ");
    output += treeToString(node->SW, prefix+"   ");
    output += treeToString(node->SE, prefix+"   ");
    return output;
}

std::string BucketQuadTree::debug() {
    std::string output = "";
    int height = calculateHeight(root);
    int numElements = calculateNumElements(root);
    output += "Number of Elements in Quad Tree: " + std::to_string(numElements) + "\n"; 
    output += "Height of the Quad Tree: " + std::to_string(height) + "\n";
    output += "Legend:\n";
    output += "N = non-bucket node\n";
    output += "B = bucket node\n";
    output += "-------------------------------------------------------------------------\n";
    output += treeToString(root, " ");
    output += "-------------------------------------------------------------------------\n";
    return output;
}

