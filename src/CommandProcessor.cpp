#include "../include/CommandProcessor.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>

Command::Command(CommandType type, std::vector<std::string> tokens) {
    Command::type = type;
    Command::tokens = tokens;
};

void Command::printCommand() {
    std::cout << Command::type << " ";
    for (int i = 0; i < Command::tokens.size(); ++i) {
        std::cout << tokens[i] << " ";
    }
    std::cout << "\n";
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
    // Doing this to avoid warning, but this should be fine. I think :D
    // It is never reached.
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

CommandProcessor::CommandProcessor() {};

void CommandProcessor::processSingleCommand(std::string command_string) {
    Command command = parser.parseCommand(command_string);
    command.printCommand(); 
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

