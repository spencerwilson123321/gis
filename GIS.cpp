#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#ifndef COMMANDPROCESSOR
#include "./include/CommandProcessor.h"
#endif

bool exists(std::string filename) {
    std::ifstream infile(filename);
    return infile.good();
}

int main(int argc, const char **argv) {
    // ./GIS <command file path> <log file path> <db file path>
    if (argc == 4) {
        std::string commandFilename(argv[2]);
        std::string logFilePath(argv[3]);
        std::string dbFilePath(argv[1]);

        // Basic error checking.
        if (!exists(commandFilename)) {
            std::cout << "Command Script File does not exist: " << "\"" << commandFilename << "\"" << std::endl;
            std::cout << "Usage: ./GIS <database file name> <command script file name> <log file name>" << std::endl;
            exit(EXIT_FAILURE);
        }

        char timeBuffer[1024];
        // std::string timeBuffer;
        time_t curr_time;
        tm* curr_tm;
        time(&curr_time);
        curr_tm = localtime(&curr_time);
        std::strftime(timeBuffer, 100, "%c", curr_tm);
        std::string student_info = "Final Project for COMP 8042\n";
        student_info += "Student Name: Spencer Wilson, Student ID: A01032190\n";
        student_info += "Database File: " + dbFilePath + "\n";
        student_info += "Script File: " + commandFilename + "\n";
        student_info += "Log File: " + logFilePath + "\n";
        student_info += "Start Time: " + std::string(timeBuffer) + "\n";

        // Creating necessary objects
        Logger logger(logFilePath);
        BufferPool bufferpool(dbFilePath);
        Node node;
        BucketQuadTree tree(&node);
        Hashtable hash;
        DatabaseManager dbmgr(dbFilePath, bufferpool, hash, tree);
        // Command processor has references
        // to all these objects so that it can
        // dispatch the proper operations.
        CommandProcessor cmdProcessor(logger, dbmgr);
        // Log student information first.
        logger.log(student_info);
        cmdProcessor.processCommandFile(commandFilename);
        exit(EXIT_SUCCESS);
    } else {
        std::cout << "Received " << argc-1 << " arguments. This program has 3 required arguments." << std::endl;
        std::cout << "Usage: ./GIS <database file name> <command script file name> <log file name>" << std::endl;
        exit(EXIT_FAILURE);
    }
};
