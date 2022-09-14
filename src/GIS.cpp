#include <iostream>
#include <string>
#include <fstream>
#include "../include/Logger.h"
#include "../include/GISRecord.h"
#include "../include/Command.h"
#include "../include/CommandProcessor.h"

int main(int argc, const char **argv) {
    // TODO: get the file path from command line arguments.
    // Parse Command Line Arguments
    if (argc == 2) {

        std::string filename(argv[1]);
        CommandProcessor cmdProcessor;
        cmdProcessor.processCommandFile(filename);
        exit(EXIT_SUCCESS);

    } else {

        exit(EXIT_FAILURE);

    }
}

