#include <string>

class Logger {

    std::string logFilePath;

    public:
        Logger();
        Logger(std::string path);
        void log(std::string message);
};