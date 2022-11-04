#include <iostream>
#include <string>
#include <fstream>
#include "../include/Logger.h"


Logger::Logger(std::string path) {
    Logger::logFilePath = path;
    std::ofstream file(logFilePath);
    if (file.is_open()) {
        file << "";
    }
    file.close();
};

void Logger::log(std::string message) {
    std::ofstream file(Logger::logFilePath, std::ios::app);
    if (file.is_open()) {
        file << message;
    }
    file.close();
};

Logger::Logger() {};
