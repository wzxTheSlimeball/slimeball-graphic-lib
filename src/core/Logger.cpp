//MIT License

//Copyright (c) 2026 Z-Multiplier
#include "Logger.hpp"
#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <memory>

void Core::logger::traceLog(enum LogLevel level,std::string message){
    std::lock_guard<std::mutex> lock(safeLock);
    auto now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    switch(level){
        case LOG_DEBUG:
            (*out)<<std::ctime(&now)<<"[DEBUG]: "<<message<<std::endl;
            break;
        case LOG_INFO:
            (*out)<<std::ctime(&now)<<"[INFO]: "<<message<<std::endl;
            break;
        case LOG_WARNING:
            (*out)<<std::ctime(&now)<<"[WARNING]: "<<message<<std::endl;
            break;
        case LOG_ERROR:
            (*out)<<std::ctime(&now)<<"[ERROR]: "<<message<<std::endl;
            break;
        case LOG_NOTE:
            (*out)<<"[NOTE]: "<<message<<std::endl;
            break;
        case LOG_FATAL:
            (*out)<<"[FATAL]: "<<message<<std::endl;
            break;
        default:
            (*out)<<"[UNKNOWN]: "<<message<<std::endl;
            break;
    }
}
