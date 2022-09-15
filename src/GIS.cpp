#include <iostream>
#include <string>
#include <fstream>
#include "../include/GISRecord.h"
#include "../include/CommandProcessor.h"

int main(int argc, const char **argv) {
    // TODO: get the file path from command line arguments.
    // Parse Command Line Arguments
    // ./GIS <command file path> <log file path> <db file path>
    // TODO take log file path as command line argument and implement writing to the log file
    if (argc == 4) {
        std::string commandFilename(argv[1]);
        std::string logFilename(argv[2]);
        std::string dbFilename(argv[3]);
        Logger logger(logFilename);
        CommandProcessor cmdProcessor(logger);
        cmdProcessor.processCommandFile(commandFilename);
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
};

