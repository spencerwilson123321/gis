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
    numOccupied = 0;
}


Hashtable::Hashtable() {
    loadFactor = 0.7f;
    numOccupied = 0;
};


// --------------- Methods -----------------------


void Hashtable::initializeTable(int n) {
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
    unsigned int len = key.size();
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
    int n = 1;
    int index = hashFunction(key);
    int alpha = 0;
    while (true) {
        alpha = ((n*n)+n)/2;
        alpha = (index + alpha) % buckets.size();
        if (bucketStatus[alpha] != OCCUPIED) {
            bucketStatus[alpha] = OCCUPIED;
            buckets[alpha] = make_pair(key, value);
            break;
        }
        n = n + 1;
    }
}


void Hashtable::debug() {
    int test = 0;
    for (int i = 0; i < buckets.size(); i++) {
        if (bucketStatus[i] == OCCUPIED) {
            test += 1;
            // std::cout << "Index: " << i << std::endl;
            // std::cout << "Key: " << "'" << buckets[i].first << "'" << std::endl;
            // std::cout << "Value: " << buckets[i].second << std::endl;
        }
    }
    std::cout << "Actual Num Occupied: " << test << std::endl;
}


void Hashtable::add(std::string key, int value) {
    if (((float)numOccupied/(float)buckets.size()) >= loadFactor) {
        rehash();
    }
    unsigned int index = hashFunction(key);
    // Check for collision.
    if (bucketStatus[index] != OCCUPIED) {
        buckets[index] = make_pair(key, value);
        bucketStatus[index] = OCCUPIED;
        numOccupied += 1;
    } else {
        collisionFunction(key, value);
        numOccupied += 1;
    }
};


void Hashtable::rehash() {
    numOccupied = 0;
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
        // The element might be in the wrong position due to probing.
        if (buckets[index].first == key) {
            // Found it.
            return true;
        // If the key doesn't match. Then check quadratically
        // for the next position.
        // If we find a position 
        } else {
            int n = 1;
            int alpha = 0;
            while (true) {
                alpha = ((n*n)+n)/2;
                alpha = (index + alpha) % buckets.size();
                if (bucketStatus[alpha] == EMPTY) {
                    // If this bucket is truly empty, then the value won't be in the list.
                    return false;
                }
                if (bucketStatus[alpha] == OCCUPIED && buckets[alpha].first == key) {
                    return true;
                }
                n = n + 1;
            }
        }
    }
    if (bucketStatus[index] == EMPTY) {
        return false;
    }
    if (bucketStatus[index] == DELETED) {
        if (buckets[index].first == key) {
            return false;
        }
        int n = 1;
        int alpha = 0;
        while (true) {
            alpha = ((n*n)+n)/2;
            alpha = (index + alpha) % buckets.size();
            if (bucketStatus[alpha] == EMPTY) {
                // If this bucket is truly empty, then the value won't be in the list.
                return false;
            }
            if (bucketStatus[alpha] == OCCUPIED && buckets[alpha].first == key) {
                return true;
            }
            n = n + 1;
        }
    }
    return false;
};


void Hashtable::remove(std::string key) {
    unsigned int index = hashFunction(key);
    bucketStatus[index] == DELETED;
};

