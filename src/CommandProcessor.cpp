#include "../include/CommandProcessor.h"
#include <string>
#include <fstream>
#include <iostream>

CommandProcessor::CommandProcessor() {};

void CommandProcessor::processCommandFile(std::string file_path) {
    // Open a file using the filename.
    std::ifstream file(file_path);
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
    } else {
        std::cout << "Could not open file.\n";
    }
}



