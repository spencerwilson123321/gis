#include <iostream>
#include <string>
#include <fstream>
#include "../include/Logger.h"

Logger::Logger() {};

void Logger::log(std::string message, std::string file_path) {
    std::ofstream file(file_path);
    if (file.is_open()) {
        file << "Log Message: " << message << std::endl;
    }
    file.close();
};

