#include "../include/CommandProcessor.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>

Command::Command(CommandType type, std::vector<std::string> args) {
    Command::type = type;
    Command::args = args;
};

CommandParser::CommandParser() {};

Command CommandParser::parseCommand(std::string command) {
    // Read the string command, and tokenize it by splitting it by
    // whitespace.
    // Read the first word (the command), and set the Command objects type
    // member to the corresponding type.
    // The rest of the words get placed into the Command objects args member.
    // Then, return the Command object.
    std::vector<std::string> tokens = tokenizeCommand(command);
    for (std::string token : tokens) {
        std::cout << token << std::endl;
    }
    return Command(COMMENT, tokens);
}

std::vector<std::string> CommandParser::tokenizeCommand(std::string command) {
    std::vector<std::string> tokens{};
    std::string temp = "";
    for (int i = 0; i < command.length(); ++i) {
        if (command[i] == ' ') {
            tokens.push_back(temp);
            temp = "";
        } else {
            temp.push_back(command[i]);
        }
    }
    tokens.push_back(temp);
    return tokens;
};

CommandProcessor::CommandProcessor() {};

void CommandProcessor::processSingleCommand(std::string command_string) {
    Command command = parser.parseCommand(command_string);
    // If command.type == A:
    //      do something (Perform some operation, and log it to the Logfile.)
    // else if command.type == B:
    //      do something else.
    // etc...
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

