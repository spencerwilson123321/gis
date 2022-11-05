#include "../include/DatabaseManager.h"

// Empty constructor
DatabaseManager::DatabaseManager() {};

// Constructor
DatabaseManager::DatabaseManager(std::string dbfilename) {
    DatabaseManager::dbfilename = dbfilename;
}

void DatabaseManager::setWestLong(float val) {
    DatabaseManager::westLong = val;
};

void DatabaseManager::setEastLong(float val) {
    DatabaseManager::eastLong = val;
};

void DatabaseManager::setSouthLat(float val) {
    DatabaseManager::southLat = val;
};

void DatabaseManager::setNorthLat(float val) {
    DatabaseManager::northLat = val;
};

float DatabaseManager::convertDMSToDD(std::string dms) {
    float result;
    int size = dms.size();
    if (dms[size-1] == 'W') {
        float seconds = std::stof(dms.substr(5, 2)) / 3600;
        float minutes = std::stof(dms.substr(3, 2)) / 60;
        result = std::stof(dms.substr(0, 3)) + minutes + seconds;
    } else {
        float seconds = std::stof(dms.substr(4, 2)) / 3600;
        float minutes = std::stof(dms.substr(2, 2)) / 60;
        result = std::stof(dms.substr(0, 2)) + minutes + seconds;
    }
    return result;
};



