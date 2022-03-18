#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <cstring>
#include <chrono>
#include <time.h>
#include <string>
#include <exception>
#include <assert.h>

namespace dailyrecord
{

  static FILE* log_file_ = nullptr;

  bool LogInit();
  bool LogClose();

  enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
  };

  static char *LEVEL[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
  };

  class Log : public std::basic_ostringstream<char> {
   public:
    Log(const char* file, int line, LogLevel level);
    ~Log();

   protected:
    void LogWrite();

   private:
    const char* name_;
    int         line_;
    LogLevel    level_;
  };

  class LogFatal : public Log {
   public:
    LogFatal(const char* file, int line);
    ~LogFatal();
  };

  #define _LOG_INFO   ::dailyrecord::Log(__FILE__, __LINE__, ::dailyrecord::LogLevel::INFO)
  #define _LOG_DEBUG  ::dailyrecord::Log(__FILE__, __LINE__, ::dailyrecord::LogLevel::DEBUG)
  #define _LOG_WARN   ::dailyrecord::Log(__FILE__, __LINE__, ::dailyrecord::LogLevel::WARN)
  #define _LOG_ERROR  ::dailyrecord::Log(__FILE__, __LINE__, ::dailyrecord::LogLevel::ERROR)
  #define _LOG_FATAL  ::dailyrecord::LogFatal(__FILE__, __LINE__, ::dailyrecord::LogLevel::FATAL)

  #define LOG(level) _LOG_##level

  #ifndef likely
  #define likely(x) __builtin_expect(!!(x), 1)
  #endif

  #ifndef unlikely
  #define unlikely(x) __builtin_expect(!!(x), 0)
  #endif

} //namespace dailyrecord
#endif //__LOG_H__