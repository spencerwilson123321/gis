#ifndef NAMEINDEX
#define NAMEINDEX
#include "../include/NameIndex.h"
#endif
#include <iostream>


// -------------- Constructors -------------------


Hashtable::Hashtable(int n) {
    buckets.resize(n);
    bucketStatus.resize(n);
    for (int i = 0; i < bucketStatus.size(); i++) {
        bucketStatus[i] = EMPTY;
    }
    loadFactor = 0.7f;
}


Hashtable::Hashtable() {
    loadFactor = 0.7f;
};


// --------------- Methods -----------------------


void Hashtable::initializeTable(int n) {
    // int c = n * 20 / 10; // Initialize to 150% of the maximum size. This is a bad choice; change it!
    buckets.resize(n);
    bucketStatus.resize(n);
    for (int i = 0; i < bucketStatus.size(); i++) {
        bucketStatus[i] = EMPTY;
    }
}


unsigned int Hashtable::hashFunction(std::string key) {
    // Using elfhash for the hash function.
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;
    unsigned int len = key.length();
    for (i = 0; i < len; i++) {
        hash = (hash << 4) + (key[i]);
		if ((x = hash & 0xF0000000) != 0)
		{
			hash ^= (x >> 24);
		}
		hash &= ~x;
    }
    return hash % Hashtable::buckets.size();
};


void Hashtable::collisionFunction(std::string key, int value) {
    // std::cout << "Resolving collision..." << std::endl;
    int n = 1;
    int index = hashFunction(key);
    int alpha = 0;
    while (true) {
        alpha = ((n*n)+n)/2;
        if (alpha > buckets.size()-1) {
            alpha = alpha-(buckets.size()-1);
        }
        if (bucketStatus[index + alpha] != OCCUPIED) {
            bucketStatus[index + alpha] == OCCUPIED;
            buckets[index + alpha] = make_pair(key, value);
            numOccupied += 1;
            break;
        }
        n = n + 1;
    }
}


void Hashtable::debug() {
    for (int i = 0; i < Hashtable::buckets.size(); i++) {
        if (Hashtable::bucketStatus[i] == OCCUPIED) {
            std::cout << "Index: " << i << std::endl;
            std::cout << "Key: " << "'" <<  Hashtable::buckets[i].first << "'" << std::endl;
            std::cout << "Value: " << Hashtable::buckets[i].second << std::endl;
        }
    }
}


void Hashtable::add(std::string key, int value) {
    if (((float)numOccupied/(float)buckets.size()) >= loadFactor) {
        rehash();
    }
    unsigned int index = hashFunction(key);
    // Check for collision.
    if (bucketStatus[index] == OCCUPIED) {
        collisionFunction(key, value);
        return;
    }
    buckets[index] = make_pair(key, value);
    bucketStatus[index] = OCCUPIED;
    numOccupied += 1;
};


void Hashtable::rehash() {
    int newSize = buckets.size()*2;
    std::vector<std::pair<std::string, int>> newBuckets;
    std::vector<BucketStatus> newBucketStatus;
    newBuckets.resize(newSize);
    newBucketStatus.resize(newSize);
    std::vector<std::pair<std::string, int>> oldBuckets = buckets;
    std::vector<BucketStatus> oldBucketStatus = bucketStatus;
    buckets = newBuckets;
    bucketStatus = newBucketStatus;
    for (int i = 0; i < bucketStatus.size(); i++) {
        bucketStatus[i] = EMPTY;
    }
    for (int i = 0; i < oldBuckets.size(); i++) {
        if (oldBucketStatus[i] == OCCUPIED) {
            add(oldBuckets[i].first, oldBuckets[i].second);
        }
    }
}


bool Hashtable::search(std::string key) {
    unsigned int index = hashFunction(key);
    if (bucketStatus[index] == OCCUPIED) {
        return true;
    }
    return false;
};


void Hashtable::remove(std::string key) {
    unsigned int index = hashFunction(key);
    bucketStatus[index] == DELETED;
};

