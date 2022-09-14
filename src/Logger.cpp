#include <iostream>
#include "../include/Logger.h"

Logger::Logger() {};

void Logger::log(const char *message) {
    std::cout << "Message: " << message << std::endl;
};
