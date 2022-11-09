#include <iostream>
#include <string>

class DatabaseManager
{
private:

    // More instance variables;
    std::string dbfilename;
public:
    DatabaseManager();
    DatabaseManager(std::string dbfilename);
    // float convertDMSToDD(std::string dms);
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

