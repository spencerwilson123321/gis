#include <iostream>
#include <string>
#include <fstream>
#include "../include/GISRecord.h"
#include "../include/CommandProcessor.h"

int main(int argc, const char **argv) {
    // ./GIS <command file path> <log file path> <db file path>
    if (argc == 4) {
        std::string commandFilename(argv[1]);
        std::string logFilename(argv[2]);
        std::string dbFilename(argv[3]);

        // Creating necessary objects
        Logger logger(logFilename);
        DatabaseManager dbmgr(dbFilename);
        
        // Command processor has references
        // to all these objects so that it can
        // dispatch the proper operations.
        CommandProcessor cmdProcessor(logger, dbmgr);
        cmdProcessor.processCommandFile(commandFilename);
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
};
