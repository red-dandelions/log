#include "log.h"

namespace dailyrecord
{

/*-----------------------------------------------------------------------------------------------*/
 bool LogInit() {
    //如果log_file_不为空，则先关闭
    if (log_file_ != nullptr) {
      LogClose();
    }
		
    //获取当前日期，将其作为log文件名
    time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const int path_buffer_size = 20;
    char path[path_buffer_size];
    strftime(path, path_buffer_size, "%Y%m%d.log", localtime(&now_time));

    //以追加的方式打开文件，供log类写入日志，抛异常则返回false
    try {
      log_file_ = fopen(path, "a");
    } catch (std::exception e) {
      std::cerr << "catch exception: " << e.what() << std::endl;
      LogClose();
      return false;
    } catch (...) {
      std::cerr << "catch unknow exception" << std::endl;
      LogClose();
      return false;
    }

    return true;
  }

  bool LogClose() {
    if (log_file_ == nullptr) {
      return true;
    }
    //关闭文件，log_file_置为nullptr，抛出异常则打印信息到stderr，返回false
    try {
      fclose(log_file_);
    } catch (std::exception e) {
      std::cerr << "catch exception: " << e.what() << std::endl;
      log_file_ = nullptr;
      return false;
    } catch (...) {
      std::cerr << "catch unknow exception" << std::endl;
      log_file_ = nullptr;
      return false;
    }
    log_file_ = nullptr;
    return true;
  }

  Log::Log(const char* name, int line, LogLevel level) : name_(name), line_(line), level_(level) { }

  void Log::LogWrite() {
    //获取当前时间然后格式化
    time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const int time_buff_size = 32;
    char time_buff[time_buff_size];
    strftime(time_buff, time_buff_size, "%Y-%m-%d %H:%M:%S", localtime(&now_time));
    //写入日志文件
    fprintf(log_file_, "%s [%s] %s:%d %s\n", time_buff, LEVEL[level_], name_, line_, str().c_str());
  }

  namespace {
    LogLevel GetLogLevel() {
      const char* log_level = getenv("LOG_LEVEL");
      if (log_level == nullptr) {
        return LogLevel::INFO;
      }
      if (strcasecmp(log_level, "DEBUG") == 0) {
        return LogLevel::DEBUG;
      }
      if (strcasecmp(log_level, "WARN") == 0) {
        return LogLevel::WARN;
      }
      if (strcasecmp(log_level, "ERROR") == 0) {
        return LogLevel::ERROR;
      }
      if (strcasecmp(log_level, "FATAL") == 0) {
        return LogLevel::FATAL;
      }
      return LogLevel::INFO;
    }
  }

  Log::~Log() {
    static LogLevel min_level = ::dailyrecord::GetLogLevel();
    if (likely(level_ >= min_level)) {
      LogWrite();
    }
  }

  LogFatal::LogFatal(const char* file, int line) : Log(file, line, LogLevel::FATAL) { }

  LogFatal::~LogFatal() {
    LogWrite();
    abort();
  }
} //namespace dailyrecord
