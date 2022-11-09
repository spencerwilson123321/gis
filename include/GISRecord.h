#include <vector>
#include <string>

class GISRecord {
    private:
        void parseEntry(std::string entry);
        std::vector<std::string> splitEntry(std::string entry);
    public:
        int featureId;
        std::string featureName;
        std::string featureClass;
        std::string stateAlpha;
        std::string stateNumeric;
        std::string countyName;
        std::string countyNumeric;
        int primaryLatitudeDMS;
        int primaryLongitudeDMS;
        float primaryLatitudeDD;
        float primaryLongitudeDD;
        int sourceLatitudeDMS;
        int sourceLongitudeDMS;
        float sourceLatitudeDD;
        float sourceLongitudeDD;
        std::string elevationMeters;
        std::string elevationFeet;
        std::string mapName;
        std::string dateCreated;
        std::string dateEdited;
        float validateDD(std::string dd);
        int convertDMSToSeconds(std::string dms);
        GISRecord();
        GISRecord(std::string entry);
};

