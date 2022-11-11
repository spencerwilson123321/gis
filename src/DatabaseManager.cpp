
#ifndef DBMGR
#define DBMGR
#include "../include/DatabaseManager.h"
#endif

#ifndef GISRECORD
#define GISRECORD
#include "../include/GISRecord.h"
#endif

#ifndef NAMEINDEX
#define NAMEINDEX
#include "../include/NameIndex.h"
#endif

#include <fstream>
#include <iostream>
#include <unistd.h>


// Empty constructor
DatabaseManager::DatabaseManager() {};


// Constructor
DatabaseManager::DatabaseManager(std::string dbfilename) {
    DatabaseManager::dbfilename = dbfilename;
}


DatabaseManager::DatabaseManager(std::string dbfilename, BufferPool pool) {
    DatabaseManager::dbfilename = dbfilename;
    DatabaseManager::pool = pool;
}


DatabaseManager::DatabaseManager(std::string dbfilename, BufferPool pool, Hashtable hash) {
    DatabaseManager::dbfilename = dbfilename;
    DatabaseManager::pool = pool;
    DatabaseManager::hash = hash;
}


void DatabaseManager::setWestLong(int val) {
    DatabaseManager::westLong = val;
};


void DatabaseManager::setEastLong(int val) {
    DatabaseManager::eastLong = val;
};


void DatabaseManager::setSouthLat(int val) {
    DatabaseManager::southLat = val;
};


void DatabaseManager::setNorthLat(int val) {
    DatabaseManager::northLat = val;
};


std::string DatabaseManager::stringWorldBoundaries() {
    std::string result = "";
    // result += "World Coordinates:\n";
    result += "----------------------------------------------------------\n";
    result += "World Boundaries\n";
    result += "----------------------------------------------------------\n";
    result += "\t    " + std::to_string(DatabaseManager::northLat) + "\n";
    result += std::to_string(DatabaseManager::westLong) + "\t\t\t" + std::to_string(DatabaseManager::eastLong) + "\n";
    result += "\t    " + std::to_string(DatabaseManager::southLat) + "\n";
    result += "----------------------------------------------------------\n";
    return result;
};


int DatabaseManager::convertDMSToSeconds(std::string dms) {
    int result = 0;
    int size = dms.size();
    if (dms == "Unknown") {
        return 0;
    }
    if (dms[size-1] == 'W') {
        int seconds = std::stoi(dms.substr(5, 2));
        int minutes = std::stoi(dms.substr(3, 2)) * 60;
        int degrees = std::stoi(dms.substr(0, 3)) * 3600;
        result = degrees + minutes + seconds;
        result *= -1;
    } else if (dms[size-1] == 'E') {
        int seconds = std::stoi(dms.substr(5, 2));
        int minutes = std::stoi(dms.substr(3, 2)) * 60;
        int degrees = std::stoi(dms.substr(0, 3)) * 3600;
        result = degrees + minutes + seconds;
    } else if (dms[size-1] == 'N') {
        int seconds = std::stof(dms.substr(4, 2));
        int minutes = std::stof(dms.substr(2, 2)) * 60;
        int degrees = std::stof(dms.substr(0, 2)) * 3600;
        result = degrees + minutes + seconds;
    } else if (dms[size-1] == 'S') {
        int seconds = std::stof(dms.substr(4, 2));
        int minutes = std::stof(dms.substr(2, 2)) * 60;
        int degrees = std::stof(dms.substr(0, 2)) * 3600;
        result = degrees + minutes + seconds;
        result *= -1;
    }
    return result;
};


std::string DatabaseManager::getFilePath() {
    return DatabaseManager::dbfilename;
}


bool DatabaseManager::importRecords(std::string path) {
    // 1. Open the database file in append mode.
    // 2. Open the import file in read mode.
    std::ofstream database_file;
    std::ifstream import_file;
    std::string buffer;
    std::string db_filepath = DatabaseManager::getFilePath();
    DatabaseManager::hash.initializeTable(1024);
    database_file.open(db_filepath, std::ios::app);
    import_file.open(path, std::ios::in);
    int first_line = 1;
    int offset = 0;
    if (import_file.is_open() && database_file.is_open()) {
        int numDroppedEntries = 0;
        int numAddedEntries = 0;
        int numOutOfBounds = 0;
        int numInBounds = 0;
        while (std::getline(import_file, buffer)) {
            if (first_line) {
                first_line = 0;
                continue;
            }
            // Calculate the file offset. This will
            // be stored inside the Data Structures
            // so that we can quickly retrieve them 
            // from the database file.
            offset += buffer.size();
            // 1. Convert buffer to GISRecord object.
            GISRecord record(buffer);
            // 2. Check that DMSLat and DMSLong are not equal to 0.
            if (record.primaryLatitudeDMS != 0 || record.primaryLongitudeDMS != 0) {
                // 3. If they aren't equal to zero, then use them to check if they are in the world boundaries.
                // 4. Check if they are in the boundaries.
                if (record.primaryLatitudeDMS < DatabaseManager::northLat &&
                    record.primaryLatitudeDMS > DatabaseManager::southLat &&
                    record.primaryLongitudeDMS < DatabaseManager::eastLong &&
                    record.primaryLongitudeDMS > DatabaseManager::westLong) {
                    numAddedEntries += 1;
                    numInBounds += 1;
                    database_file << buffer << std::endl;
                    // Now we must fill out the data structures.
                    // 1. Fill out the hash table.
                    if (record.featureName == "" || record.stateAlpha == "") {
                        std::cout << "Missing feature name or state abbreviation!" << std::endl;
                        import_file.close();
                        database_file.close();
                        exit(1);
                    }
                    std::string featureNamestateAbbreviation = record.featureName + record.stateAlpha;
                    DatabaseManager::hash.insert(featureNamestateAbbreviation, offset);
                } else {
                    numOutOfBounds += 1;
                    numDroppedEntries += 1;
                }
            } else {
                // Ignore the entry if it is missing DMS coordinates.
                numDroppedEntries += 1;
            }
        }
        std::cout << "Number of dropped entries: " << numDroppedEntries << std::endl;
        std::cout << "Number of added entries: " << numAddedEntries << std::endl;
        std::cout << "Number of Out-of-Bound entries: " << numOutOfBounds << std::endl;
        std::cout << "Number of In-Bound entries: " << numInBounds << std::endl;
    }
    import_file.close();
    database_file.close();
    return true;
}



