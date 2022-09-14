#include <string>
#include <vector>
#include "../include/Logger.h"

enum CommandType {
    IMPORT,
    DEBUG,
    WHAT_IS_AT,
    WHAT_IS_IN,
    WHAT_IS,
    QUIT,
    COMMENT
};

class Command {

    CommandType type;
    std::vector<std::string> args;

    public:
        Command(CommandType type, std::vector<std::string> args);

};

class CommandParser {

    public:
        CommandParser();
        Command parseCommand(std::string command);

};

class CommandProcessor {
    
    CommandParser parser;
    Logger logger;
    
    void processSingleCommand(std::string command);
    
    public:
        CommandProcessor();
        void processCommandFile(std::string file_path);
        void setCommandParser();
};

