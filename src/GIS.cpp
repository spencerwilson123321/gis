#include <iostream>
#include <string>
#include <fstream>

#ifndef COMMANDPROCESSOR
#include "../include/CommandProcessor.h"
#endif

int main(int argc, const char **argv) {
    // ./GIS <command file path> <log file path> <db file path>
    if (argc == 4) {
        std::string commandFilename(argv[1]);
        std::string logFilePath(argv[2]);
        std::string dbFilePath(argv[3]);

        // Creating necessary objects
        Logger logger(logFilePath);
        BufferPool bufferpool(dbFilePath);
        DatabaseManager dbmgr(dbFilePath, bufferpool);

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
