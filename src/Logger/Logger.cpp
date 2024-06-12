#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#define RED "\e[0;31m"
#define GRN "\e[0;32m"

std::vector<LogEntry> Logger::messages;

std::string DateTimeToString(){
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%y %H:%M:%S", std::localtime(&now));
    return output;
}

void Logger::Log(std::string& msg){
    LogEntry log;
    log.type = LOG_INFO;
    log.message = DateTimeToString() +" | "+ msg;
    std::cout << GRN "LOG | "+ log.message << std::endl;
    messages.push_back(log);
}

void Logger::Log(const std::string& msg){
    LogEntry log;
    log.type = LOG_INFO;
    log.message = DateTimeToString() +" | "+ msg;
    std::cout << GRN "LOG | "+ log.message << std::endl;
    messages.push_back(log);
}

void Logger::Err(std::string& msg){
    LogEntry log;
    log.type = LOG_ERROR;
    log.message = DateTimeToString() +" | "+ msg;
    std::cerr << RED "Err | "+ log.message << std::endl;
    messages.push_back(log);
}

void Logger::Err(const std::string& msg){
    LogEntry log;
    log.type = LOG_ERROR;
    log.message = DateTimeToString() +" | "+ msg;
    std::cerr << RED "Err | "+ log.message << std::endl;
    messages.push_back(log);
}

