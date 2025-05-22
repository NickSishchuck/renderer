#ifdef _WIN32
    #include <windows.h>
    #undef ERROR  // Remove Windows ERROR macro
    #undef DEBUG  // Remove Windows DEBUG macro (if it exists)
#endif

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "GL/gl.h"

class Logger {
private:
    static Logger* instance;
    std::ofstream logFile;
    std::string logFileName;
    std::string basePath;
    bool initialized;

    // Log level settings
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        TODO
    };
    LogLevel currentLevel;

    // Display settings
    bool showTimestamps;
    bool showSourceInfo;
    bool useColors;

    Logger();

    // Helper functions
    std::string getLogLevelString(LogLevel level);
    std::string getCurrentTimestamp();
    std::string createLogFileName();

    // Internal logging function
    void logInternal(LogLevel level, const std::string& message, const char* file, int line);

public:
    // Singleton access
    static Logger* getInstance();

    ~Logger();

    bool init();

    // Settings functions
    void setLogLevel(LogLevel level);
    void enableTimestamps(bool enable);
    void enableSourceInfo(bool enable);
    void enableColors(bool enable);
    void setBasePath(const std::string& path);

    // Logging functions
    void debug(const std::string& message, const char* file = nullptr, int line = 0);
    void info(const std::string& message, const char* file = nullptr, int line = 0);
    void warning(const std::string& message, const char* file = nullptr, int line = 0);
    void error(const std::string& message, const char* file = nullptr, int line = 0);
    void fatal(const std::string& message, const char* file = nullptr, int line = 0);
    void todo(const std::string& message, const char* file = nullptr, int line = 0);

    std::string glErrorToString(GLenum error);
};

// Convenient macros for logging
#define LOG_DEBUG(msg) Logger::getInstance()->debug(msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::getInstance()->info(msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::getInstance()->warning(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::getInstance()->error(msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) Logger::getInstance()->fatal(msg, __FILE__, __LINE__)
#define LOG_TODO(msg) Logger::getInstance()->todo(msg, __FILE__, __LINE__)

// Macro for OpenGL error logging
#define LOG_GLERROR(context) { \
    GLenum glErr = glGetError(); \
    if (glErr != GL_NO_ERROR) { \
        std::string errorMsg = std::string(context) + ": " + Logger::getInstance()->glErrorToString(glErr); \
        Logger::getInstance()->error(errorMsg, __FILE__, __LINE__); \
    } \
}

#endif
