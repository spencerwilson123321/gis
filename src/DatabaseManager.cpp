
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
#include <string>
#include <sstream>


// Empty constructor
DatabaseManager::DatabaseManager() {};


// Constructor
DatabaseManager::DatabaseManager(std::string dbfilename) {
    DatabaseManager::dbfilename = dbfilename;
};


DatabaseManager::DatabaseManager(std::string dbfilename, BufferPool pool) {
    DatabaseManager::dbfilename = dbfilename;
    DatabaseManager::pool = pool;
};


DatabaseManager::DatabaseManager(std::string dbfilename, BufferPool pool, Hashtable hash, BucketQuadTree tree) {
    DatabaseManager::dbfilename = dbfilename;
    DatabaseManager::pool = pool;
    DatabaseManager::hash = hash;
    DatabaseManager::quad = tree;
};


void DatabaseManager::setWestLong(int val) {
    westLong = val;
};


void DatabaseManager::setEastLong(int val) {
    eastLong = val;
};


void DatabaseManager::setSouthLat(int val) {
    southLat = val;
};


void DatabaseManager::setNorthLat(int val) {
    northLat = val;
};

void DatabaseManager::setInitialQuadTreeBoundaries() {
    quad.setRootBoundaries(westLong, eastLong, northLat, southLat);
}


std::string DatabaseManager::stringWorldBoundaries() {
    std::string result = "";
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

std::string DatabaseManager::debugHash() {
    return DatabaseManager::hash.debug();
}

std::string DatabaseManager::debugPool() {
    return DatabaseManager::pool.debugPool();
}

std::string DatabaseManager::debugQuad() {
    return quad.debug();
}

std::string DatabaseManager::what_is(std::string key) {
    // 1. Retrieve the file offset from the hash table.
    int offset = DatabaseManager::hash.get(key);
    if (offset == -1) {
        return "Not found!\n";
    }
    GISRecord record = DatabaseManager::pool.retrieveRecord(offset);
    std::string output = "";
    output += std::to_string(offset)+ ": ";
    output += record.countyName + " (";
    output += std::to_string(record.primaryLatitudeDMS) + " seconds, ";
    output += std::to_string(record.primaryLongitudeDMS) + " seconds)\n";
    return output;
}

std::string DatabaseManager::what_is_at(int latitude, int longitude) {
    std::string output = "";
    Coordinate coords(latitude, longitude);
    std::vector<int> offsets = quad.search(quad.root, coords);
    if (offsets.size() == 0) {
        output += "No records found!\n";
    }
    for (int offset : offsets) {
        auto record = pool.retrieveRecord(offset);
        output += std::to_string(offset) + " | " + "'" + record.featureName + "'" + " " + "'" + record.countyName + "'" + " " + "'" + record.stateAlpha + "'\n";
    }
    return output;
}

bool isStructure(std::string type) {
    if (type == "Airport") {
        return true;
    }
    if (type == "Bridge") {
        return true;
    }
    if (type == "Building") {
        return true;
    }
    if (type == "Church") {
        return true;
    }
    if (type == "Dam") {
        return true;
    }
    if (type == "Hospital") {
        return true;
    }
    if (type == "Levee") {
        return true;
    }
    if (type == "Park") {
        return true;
    }
    if (type == "Post Office") {
        return true;
    }
    if (type == "School") {
        return true;
    }
    if (type == "Tower") {
        return true;
    }
    if (type == "Tunnel") {
        return true;
    }
    return false;
}

bool isPop(std::string type) {
    if (type == "Populated Place") {
        return true;
    }
    return false;
}

bool isWater(std::string type) {
    if (type == "Arroyo") {
        return true;
    }
    if (type == "Bay") {
        return true;
    }
    if (type == "Basin") {
        return true;
    }
    if (type == "Bend") {
        return true;
    }
    if (type == "Canal") {
        return true;
    }
    if (type == "Channel") {
        return true;
    }
    if (type == "Falls") {
        return true;
    }
    if (type == "Glacier") {
        return true;
    }
    if (type == "Gut") {
        return true;
    }
    if (type == "Harbor") {
        return true;
    }
    if (type == "Lake") {
        return true;
    }
    if (type == "Rapids") {
        return true;
    }
    if (type == "Reservoir") {
        return true;
    }
    if (type == "Sea") {
        return true;
    }
    if (type == "Spring") {
        return true;
    }
    if (type == "Stream") {
        return true;
    }
    if (type == "Swamp") {
        return true;
    }
    if (type == "Well") {
        return true;
    }
    return false;
}

std::string DatabaseManager::what_is_in(int latitude, int longitude, int halfHeight, int halfWidth, int longFlag, int filterFlag, std::string filterString) {
    std::string output = "";
    std::string prepend = "";
    int numRecordsFound = 0;
    Coordinate coord(latitude, longitude);
    std::vector<int> offsets = quad.searchRegion(quad.root, coord, halfHeight, halfWidth);
    for (int offset : offsets) {
        auto record = pool.retrieveRecord(offset);
        if (longFlag == 1) {
            // More detailed output.
            numRecordsFound += 1;
            output += "File Offset: " + std::to_string(offset) + "\n";
            output += "Feature ID: " + std::to_string(record.featureId) + "\n";
            output += "Feature Name: " + record.featureName + "\n";
            output += "Feature Class: " + record.featureClass + "\n";
            output += "State: " + record.stateAlpha + "\n";
            output += "County: " + record.countyName + "\n";
            output += "Longitude: " + std::to_string(record.primaryLongitudeDMS) + "\n";
            output += "Latitude: " + std::to_string(record.primaryLatitudeDMS) + "\n";
            output += "Elevation (ft): " + record.elevationFeet + "\n";
            output += "Map Name: " + record.mapName + "\n";
            output += "Date Created: " + record.dateCreated + "\n\n";
        } else if (filterFlag == 1) {
            // Filter the output.
            // pop | water | structure
            if (filterString == "structure") {
                if (isStructure(record.featureClass)) {
                    output += std::to_string(offset) + " | " + record.featureName + " " + record.stateAlpha + " Lat: " + std::to_string(record.primaryLatitudeDMS) + " Long: " + std::to_string(record.primaryLongitudeDMS) + "\n";
                    numRecordsFound += 1;
                }
            }
            if (filterString == "pop") {
                if (isPop(record.featureClass)) {
                    output += std::to_string(offset) + " | " + record.featureName + " " + record.stateAlpha + " Lat: " + std::to_string(record.primaryLatitudeDMS) + " Long: " + std::to_string(record.primaryLongitudeDMS) + "\n";
                    numRecordsFound += 1;
                }
            }
            if (filterString == "water") {
                if (isWater(record.featureClass)) {
                    output += std::to_string(offset) + " | " + record.featureName + " " + record.stateAlpha + " Lat: " + std::to_string(record.primaryLatitudeDMS) + " Long: " + std::to_string(record.primaryLongitudeDMS) + "\n";
                    numRecordsFound += 1;
                }
            }
        } else {
            // Regular output.
            output += std::to_string(offset) + " | " + record.featureName + " " + record.stateAlpha + " Lat: " + std::to_string(record.primaryLatitudeDMS) + " Long: " + std::to_string(record.primaryLongitudeDMS) + "\n";
            numRecordsFound += 1;
        }
    }
    if (offsets.size() == 0) {
        output += "No records found!\n";
    }  else {
        prepend = std::to_string(numRecordsFound) + " records found!\n\n";
    }
    return prepend + output;
};


std::string DatabaseManager::importRecords(std::string path) {
    // 1. Open the database file in append mode.
    // 2. Open the import file in read mode.
    std::ofstream database_file;
    std::ifstream import_file;
    std::string buffer;
    std::string output = "";
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
                offset += 1;
                continue;
            }
            // Calculate the file offset. This will
            // be stored inside the Data Structures
            // so that we can quickly retrieve them 
            // from the database file.
            // offset += buffer.size();
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
                    std::string featureNamestateAbbreviation = record.featureName + ":" + record.stateAlpha;
                    hash.add(featureNamestateAbbreviation, offset);
                    Coordinate coords(record.primaryLatitudeDMS, record.primaryLongitudeDMS);
                    quad.insert(quad.root, coords, offset);
                } else {
                    numOutOfBounds += 1;
                    numDroppedEntries += 1;
                }
            } else {
                // Ignore the entry if it is missing DMS coordinates.
                numDroppedEntries += 1;
            }
            offset += 1;
        }
        output += "Number of dropped entries: " + std::to_string(numDroppedEntries) + "\n";
        output += "Number of added entries: " + std::to_string(numAddedEntries) + "\n";
        output += "Number of Out-of-Bound entries: " + std::to_string(numOutOfBounds) + "\n";
        output += "Number of In-Bound entries: " + std::to_string(numInBounds) + "\n";
    }
    import_file.close();
    database_file.close();
    // There is a bug where the newline at the end of the database file crashes
    // the program. To make sure that doesn't happen we have to read the database file again.
    // and remove the newline at the end.
    std::ifstream fileIn(db_filepath);
    std::stringstream streamBuffer;
    std::string contents;
    if (!fileIn.is_open()) {
        std::cout << "Failed to open database file when trying to remove trailing newline.\n";
        exit(1);
    };
    streamBuffer << fileIn.rdbuf();
    contents = streamBuffer.str();
    contents.pop_back();
    fileIn.close();
    std::ofstream fileOut(db_filepath, std::ios::trunc);
    fileOut << contents;
    fileOut.close();
    return output;
}



