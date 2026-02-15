//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>
using std::ostream;
using std::string;
using std::mutex;
namespace Core{
    struct logger{
        private:
            ostream *out;
            mutex safeLock;
            std::chrono::steady_clock::time_point startTime;
        public:
            enum LogLevel{
                LOG_DEBUG,
                LOG_INFO,
                LOG_WARNING,
                LOG_ERROR,
                LOG_NOTE,
                LOG_FATAL
            };
            logger():out(&std::clog){};
            logger(ostream &output):out(&output){};
            void traceLog(enum LogLevel level,string message);
            template<typename T>
            void varLog(enum LogLevel level,string varName,T varValue){
                std::lock_guard<mutex> lock(safeLock);
                auto now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                switch(level){
                    case LOG_DEBUG:
                        (*out)<<std::ctime(&now)<<"[DEBUG]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                    case LOG_INFO:
                        (*out)<<std::ctime(&now)<<"[INFO]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                    case LOG_WARNING:
                        (*out)<<std::ctime(&now)<<"[WARNING]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                    case LOG_ERROR:
                        (*out)<<std::ctime(&now)<<"[ERROR]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                    case LOG_NOTE:
                        (*out)<<"[NOTE]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                    case LOG_FATAL:
                        (*out)<<"[FATAL]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                    default:
                        (*out)<<"[UNKNOWN]: "<<varName<<" = "<<varValue<<std::endl;
                        break;
                }
            }
            template<typename... Args>
            void formatLog(enum LogLevel level,string format,Args... args){
                std::lock_guard<mutex> lock(safeLock);
                size_t size=snprintf(nullptr,0,format.c_str(),args...)+1;
                std::unique_ptr<char[]> buf(new char[size]);
                snprintf(buf.get(),size,format.c_str(),args...);
                auto now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                switch(level){
                    case LOG_DEBUG:
                        (*out)<<std::ctime(&now)<<"[DEBUG]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                    case LOG_INFO:
                        (*out)<<std::ctime(&now)<<"[INFO]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                    case LOG_WARNING:
                        (*out)<<std::ctime(&now)<<"[WARNING]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                    case LOG_ERROR:
                        (*out)<<std::ctime(&now)<<"[ERROR]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                    case LOG_NOTE:
                        (*out)<<"[NOTE]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                    case LOG_FATAL:
                        (*out)<<"[FATAL]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                    default:
                        (*out)<<"[UNKNOWN]: "<<string(buf.get(),buf.get()+size-1)<<std::endl;
                        break;
                }
            }
    };
}
#endif // LOGGER_HPP