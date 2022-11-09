#include <iostream>
#define GISRECORD 1
#include "../include/GISRecord.h"
#include <stdlib.h>

std::vector<std::string> GISRecord::splitEntry(std::string entry) {
    std::vector<std::string> tokens;
    std::string delimiter = "|";
    size_t pos = 0;
    while ((pos = entry.find(delimiter)) != std::string::npos) {
        tokens.push_back(entry.substr(0, pos));
        entry.erase(0, pos + delimiter.length());
    }
    return tokens;
};

int GISRecord::convertDMSToSeconds(std::string dms) {
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

float GISRecord::validateDD(std::string dd) {
    if (dd == "Unknown") {
        return 0;
    }
    if (dd.size() == 0) {
        return 0;
    }
    return std::stof(dd);
}

void GISRecord::parseEntry(std::string entry) {

    std::vector<std::string> tokens = GISRecord::splitEntry(entry);
    
    GISRecord::featureId = std::stoi(tokens[0]);
    GISRecord::featureName = tokens[1];
    GISRecord::featureClass = tokens[2];
    GISRecord::stateAlpha = tokens[3];
    GISRecord::stateNumeric = tokens[4];
    GISRecord::countyName = tokens[5];
    GISRecord::countyNumeric = tokens[6];
    
    GISRecord::primaryLatitudeDMS = GISRecord::convertDMSToSeconds(tokens[7]);
    GISRecord::primaryLongitudeDMS = GISRecord::convertDMSToSeconds(tokens[8]);

    GISRecord::primaryLatitudeDD = GISRecord::validateDD(tokens[9]);
    GISRecord::primaryLatitudeDD = GISRecord::validateDD(tokens[10]);

    GISRecord::sourceLatitudeDMS = GISRecord::convertDMSToSeconds(tokens[11]);
    GISRecord::sourceLongitudeDMS = GISRecord::convertDMSToSeconds(tokens[12]);

    GISRecord::primaryLatitudeDD = GISRecord::validateDD(tokens[13]);
    GISRecord::primaryLatitudeDD = GISRecord::validateDD(tokens[14]);

    GISRecord::elevationMeters = tokens[15];
    GISRecord::elevationFeet = tokens[16];
    GISRecord::mapName = tokens[17];
    GISRecord::dateCreated = tokens[18];
    GISRecord::dateEdited = tokens[19];
};

// Default constructor
GISRecord::GISRecord() {};

// Constructor
GISRecord::GISRecord(std::string entry) {
    GISRecord::parseEntry(entry);
};



