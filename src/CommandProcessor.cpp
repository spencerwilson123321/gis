#define COMMANDPROCESSOR 1
#include "../include/CommandProcessor.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

// ------------------------- STRING CONSTANTS --------------------------

std::string DEBUG_HASH = "hash";
std::string DEBUG_WORLD = "world";
std::string DEBUG_QUAD = "quad";
std::string DEBUG_POOL = "pool";


// ------------------------- COMMAND OBJECT ----------------------------

Command::Command(CommandType type, std::vector<std::string> tokens) {
    Command::type = type;
    Command::tokens = tokens;
};


std::string Command::getCommandString() {
    std::ostringstream oss;
    for (int i = 0; i < Command::tokens.size(); i++) {
        oss << tokens[i] << " ";
    }
    oss << "\n";
    return oss.str();
};


CommandType Command::getCommandType() {
    return type;
}


void Command::printCommand() {
    std::cout << Command::type << " ";
    for (int i = 0; i < Command::tokens.size(); ++i) {
        std::cout << tokens[i] << " ";
    }
    std::cout << "\n";
};

// ---------------------- COMMAND PARSER ------------------------------

CommandParser::CommandParser() {};


Command CommandParser::parseCommand(std::string command) {
    std::vector<std::string> tokens = tokenizeCommand(command);
    std::string first_token = tokens.at(0);
    if (first_token == ";") {
        return Command(COMMENT, tokens);
    }
    if (first_token == "debug") {
        return Command(DEBUG, tokens);
    }
    if (first_token == "import") {
        return Command(IMPORT, tokens);
    }
    if (first_token == "world") {
        return Command(WORLD, tokens);
    }
    if (first_token == "what_is") {
        return Command(WHAT_IS, tokens);
    }
    if (first_token == "what_is_at") {
        return Command(WHAT_IS_AT, tokens);
    }
    if (first_token == "what_is_in") {
        return Command(WHAT_IS_IN, tokens);
    } 
    if (first_token == "quit") {
        return Command(QUIT, tokens);
    }
    return Command(COMMENT, tokens); 
}


std::vector<std::string> CommandParser::tokenizeCommand(std::string command) {
    std::vector<std::string> tokens{};
    std::string temp = "";
    for (int i = 0; i < command.length(); ++i) {
        if (command[i] == ' ' || command[i] == '\t' || command[i] == '\n') {
            tokens.push_back(temp);
            temp = "";
        } else {
            temp.push_back(command[i]);
        }
    }
    tokens.push_back(temp);
    return tokens;
};


// ---------------------- COMMAND PROCESSOR ------------------------------


CommandProcessor::CommandProcessor(Logger log) {
    CommandProcessor::logger = log;
};

CommandProcessor::CommandProcessor(Logger log, DatabaseManager dbmgr) {
    CommandProcessor::logger = log;
    CommandProcessor::dbmgr = dbmgr;
};

CommandProcessor::CommandProcessor() {};


void CommandProcessor::setLogger(Logger log) {
    CommandProcessor::logger = log;
};


void CommandProcessor::processSingleCommand(std::string command_string) {
    Command command = parser.parseCommand(command_string);
    if (command.getCommandType() == WORLD) {
        // 1. Convert the DMS coordinates into seconds.
        // 2. Set the DatabaseManager world coordinates.
        CommandProcessor::dbmgr.setWestLong(CommandProcessor::dbmgr.convertDMSToSeconds(command.tokens[1]));
        CommandProcessor::dbmgr.setEastLong(CommandProcessor::dbmgr.convertDMSToSeconds(command.tokens[2]));
        CommandProcessor::dbmgr.setSouthLat(CommandProcessor::dbmgr.convertDMSToSeconds(command.tokens[3]));
        CommandProcessor::dbmgr.setNorthLat(CommandProcessor::dbmgr.convertDMSToSeconds(command.tokens[4]));
        std::string output = CommandProcessor::dbmgr.stringWorldBoundaries();
        logger.log(command.getCommandString());
        logger.log(output);
    }
    if (command.getCommandType() == COMMENT) {
        logger.log(command.getCommandString());
    }
    if (command.getCommandType() == IMPORT) {
        logger.log(command.getCommandString());
        std::string output = CommandProcessor::dbmgr.importRecords(command.tokens[1]);
        logger.log(output);
    }
    if (command.getCommandType() == DEBUG) {
        logger.log(command.getCommandString());
        if (command.tokens[1] == DEBUG_HASH) {
            logger.log(CommandProcessor::dbmgr.debugHash());
        }
        if (command.tokens[1] == DEBUG_WORLD) {
            logger.log("Not implemented.\n");
        }
        if (command.tokens[1] == DEBUG_QUAD) {
            logger.log("Not implemented.\n");
        }
        if (command.tokens[1] == DEBUG_POOL) {
            logger.log("Not implemented.\n");
        }
    }
    if (command.getCommandType() == WHAT_IS) {
        logger.log(command.getCommandString());
        std::string featureName = "";
        int i = 1;
        for (i; i < command.tokens.size()-1; i++) {
            if (i == command.tokens.size()-2) {
                featureName += command.tokens[i];
                break;
            }
            featureName += command.tokens[i] + " ";
        };
        std::string input = featureName + ":" + command.tokens[i+1];
        std::string output = CommandProcessor::dbmgr.what_is(input);
    }
    if (command.getCommandType() == QUIT) {
        logger.log(command.getCommandString());
        logger.log("Quitting...");
        exit(0);
    }
}


void CommandProcessor::processCommandFile(std::string file_path) {
    // Open a file using the filename.
    std::ifstream file(file_path);
    std::string command;
    if (file.is_open()) {
        while (std::getline(file, command)) {
            processSingleCommand(command);
        }
    } else {
        std::cout << "Could not open file.\n";
    }
}

