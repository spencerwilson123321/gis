#include <iostream>
#include <string>

#ifndef BUFFERPOOL
#include "../include/BufferPool.h"
#endif

class DatabaseManager
{
private:
    std::string dbfilename;
    BufferPool pool;
public:
    DatabaseManager();
    DatabaseManager(std::string dbfilename);
    DatabaseManager(std::string dbfilename, BufferPool pool);
    int convertDMSToSeconds(std::string dms);
    void printWorldBoundaries();
    std::string getFilePath();
    bool importRecords(std::string path);
    void setWestLong(int val);
    void setEastLong(int val);
    void setSouthLat(int val);
    void setNorthLat(int val);
    // The database manager needs to know the world coordinates.
    // These coordinates must be stored in Decimal Degrees (DD) format.
    // They are originally stored in DMS format so a conversion must take place
    // before these variables can be set.
    float westLong;
    float eastLong;
    float southLat;
    float northLat;
};

