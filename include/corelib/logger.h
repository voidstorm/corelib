#pragma once
#include "corelib_api.h"
#include <memory>
#include <exception>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <mutex>
#include <chrono>
#include <array>
#include <iomanip>
#include <ctime>
#include <optional>



namespace scl {

enum class LogLevel : unsigned {
   LOG_INFO=0,
   LOG_WARNING,
   LOG_ERROR
};


inline auto date_str= []()->std::string {
   std::stringstream ss;
   auto time = std::chrono::system_clock::now();
   auto time_c = std::chrono::system_clock::to_time_t(time);
   struct tm newtime;
   localtime_s(&newtime, &time_c);
   ss << std::put_time(&newtime, "%F %T");
   return ss.str();
};

inline auto info_str= []()->std::string {
   return std::string("[INFO] ");
};

inline auto warning_str = []()->std::string {
   return std::string("[WARNING] ");
};

inline auto error_str = []()->std::string {
   return std::string("[ERROR] ");
};

inline auto empty_str = []()->std::string {
   return std::string();
};

class Logger final {
public:
   static const unsigned MAX_LINE_LEN = 2048;

   Logger(const std::string & logfile, bool streamToStdOut=false, bool breakOnWarn=false, bool breakOnError=false) :
      mStreamToStdOut(streamToStdOut),
      mBreakOnWarn(breakOnWarn),
      mBreakOnError(breakOnError){
      mLog.open(logfile, std::ofstream::out | std::ofstream::trunc);
      if (mLog.fail()) {
         V(std::string("Failed to open log file: ") + logfile);
         mLog.close();
      }
   }
   
   Logger(const Logger&) = delete;
   Logger(Logger&&) = delete;
   Logger& operator=(const Logger&) = delete;
   Logger& operator=(Logger&&) = delete;


   ~Logger() {
      mLog.close();
   }

   template<class... Args>
   void log(LogLevel level, const std::optional<std::function<std::string(void)>> &annotation, const std::string &msg, Args&&... args ) {
      std::lock_guard<decltype(mMutex)> lock(mMutex);
      if (mLog.is_open()) {
         snprintf(mBuffer.data(), MAX_LINE_LEN, msg.c_str(), args...);
         {
            std::stringstream ss;
            ss << (annotation.has_value() ? annotation.value()() : "")
               << mBuffer.data()
               << std::endl;
            mLog << ss.str();
            if (mStreamToStdOut) {
               std::cout << ss.str();
            }
         }
         mLog.flush();
      }
      if (mBreakOnWarn && level == LogLevel::LOG_WARNING) {
         _CrtDbgBreak();
      }
      if (mBreakOnError && level == LogLevel::LOG_ERROR) {
         _CrtDbgBreak();
      }
   }

   template<class... Args>
   void info(const std::optional<std::function<std::string(void)>> &annotation, const std::string &msg, Args&&... args) {
      log(LogLevel::LOG_INFO, annotation, msg, args...);
   }

   template<class... Args>
   void warn(const std::optional<std::function<std::string(void)>> &annotation, const std::string &msg, Args&&... args) {
      log(LogLevel::LOG_WARNING, annotation, msg, args...);
   }

   template<class... Args>
   void error(const std::optional<std::function<std::string(void)>> &annotation, const std::string &msg, Args&&... args) {
      log(LogLevel::LOG_ERROR, annotation, msg, args...);
   }

   template<class... Args>
   void print(const std::optional<std::function<std::string(void)>> &annotation, const std::string &msg, Args&&... args) {
      log(LogLevel::LOG_INFO, annotation, msg, args...);
   }


private:
   std::array<char, MAX_LINE_LEN> mBuffer;
   std::recursive_mutex mMutex;
   std::ofstream mLog;
   bool mStreamToStdOut=false;
   bool mBreakOnWarn = false; 
   bool mBreakOnError = false;
};
}
