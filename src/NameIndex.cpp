#ifndef NAMEINDEX
#define NAMEINDEX
#include "../include/NameIndex.h"
#endif

#include <iostream>

Hashtable::Hashtable(int n) {
    int c = n * 20 / 10; // Initialize to 150% of the maximum size. This is a bad choice; change it!
    buckets.resize(c);
    bucketStatus.resize(c);
    for (int i = 0; i < bucketStatus.size(); i++) {
        bucketStatus[i] = EMPTY;
    }
}

Hashtable::Hashtable() {};

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

void Hashtable::collisionFunction(std::string key) {
    
}


void Hashtable::debug() {
    for (int i = 0; i < Hashtable::buckets.size(); i++) {
        if (Hashtable::bucketStatus[i] == OCCUPIED) {
            std::cout << Hashtable::buckets[i] << std::endl;
        }
    }
}


int Hashtable::getValue(std::string key) {
    unsigned int index = Hashtable::hashFunction(key);
    if (Hashtable::bucketStatus[index] == EMPTY || Hashtable::bucketStatus[index] == DELETED) {
        return -1;
    }
    int value = Hashtable::buckets[index];
    return value;
}


void Hashtable::insert(std::string key, int value) {
    unsigned int index = Hashtable::hashFunction(key);
    Hashtable::buckets[index] = value;
    Hashtable::bucketStatus[index] = OCCUPIED;
};


bool Hashtable::search(std::string key) {
    unsigned int index = Hashtable::hashFunction(key);
    if (Hashtable::bucketStatus[index] == OCCUPIED) {
        return true;
    }
    return false;
};


bool Hashtable::erase(std::string key) {
    return true;
};


int Hashtable::getNumCollisions() {
    return true;
};

