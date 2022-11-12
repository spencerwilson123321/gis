#include <iostream>
#include <string>

#ifndef BUFFERPOOL
#define BUFFERPOOL
#include "../include/BufferPool.h"
#endif

#ifndef NAMEINDEX
#define NAMEINDEX
#include "../include/NameIndex.h"
#endif

class DatabaseManager {
private:
    std::string dbfilename;
    BufferPool pool;
    Hashtable hash;
public:
    DatabaseManager();
    DatabaseManager(std::string dbfilename);
    DatabaseManager(std::string dbfilename, BufferPool pool);
    DatabaseManager(std::string dbfilename, BufferPool pool, Hashtable hash);
    int convertDMSToSeconds(std::string dms);
    std::string stringWorldBoundaries();
    std::string getFilePath();
    std::string importRecords(std::string path);
    std::string what_is(std::string input);
    void setWestLong(int val);
    void setEastLong(int val);
    void setSouthLat(int val);
    void setNorthLat(int val);
    std::string debugHash();
    std::string debugPool();
    // The database manager needs to know the world coordinates.
    // These coordinates must be stored in Decimal Degrees (DD) format.
    // They are originally stored in DMS format so a conversion must take place
    // before these variables can be set.
    int westLong;
    int eastLong;
    int southLat;
    int northLat;
};
