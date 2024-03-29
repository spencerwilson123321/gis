#include <string>
#include <vector>
#include "../include/Logger.h"
#ifndef DBMGR
#include "../include/DatabaseManager.h"
#endif

enum CommandType {
    IMPORT,
    DEBUG,
    WORLD,
    WHAT_IS_AT,
    WHAT_IS_IN,
    WHAT_IS,
    QUIT,
    COMMENT
};


class Command {


    public:
        Command(CommandType type, std::vector<std::string> args);
        void printCommand();
        std::string getCommandString();
        CommandType getCommandType();
        CommandType type;
        std::vector<std::string> tokens;
};

class CommandParser {

    public:
        CommandParser();
        Command parseCommand(std::string command);
        std::vector<std::string> tokenizeCommand(std::string command);

};

class CommandProcessor {
    
    CommandParser parser;
    Logger logger;
    DatabaseManager dbmgr;
    int commandNumber;
    
    void processSingleCommand(std::string command);
    
    public:
        CommandProcessor(Logger logger, DatabaseManager dbmgr);
        CommandProcessor(Logger logger);
        CommandProcessor();
        void processCommandFile(std::string file_path);
        void setCommandParser();
        void setLogger(Logger logger);
};

