#include "../include/Logger.h"
#include <filesystem>
#include <iostream>
#include <iomanip>

// Initialize the static instance pointer
Logger* Logger::instance = nullptr;

Logger::Logger() :
    initialized(false),
    currentLevel(LogLevel::INFO),
    showTimestamps(true),
    showSourceInfo(true) {
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}

bool Logger::init() {
    if (initialized) {
        return true;
    }

    // Create logs directory if it doesn't exist
    std::filesystem::create_directory("logs");

    // Create log filename with the format DD_MM_YYYY_|_HH:MM
    logFileName = createLogFileName();

    // Open log file
    logFile.open("logs/" + logFileName + ".log", std::ios::out);

    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: logs/" << logFileName << ".log" << std::endl;
        return false;
    }

    initialized = true;

    // Log initial message
    info("Logger initialized: " + logFileName);

    return true;
}

std::string Logger::createLogFileName() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(2) << localTime->tm_mday << "_"
       << std::setw(2) << (localTime->tm_mon + 1) << "_"
       << (localTime->tm_year + 1900) << "_|_"
       << std::setw(2) << localTime->tm_hour << ":"
       << std::setw(2) << localTime->tm_min;

    return ss.str();
}

std::string Logger::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::stringstream ss;
    ss << std::setfill('0')
       << "["
       << std::setw(2) << localTime->tm_hour << ":"
       << std::setw(2) << localTime->tm_min << ":"
       << std::setw(2) << localTime->tm_sec
       << "]";

    return ss.str();
}

std::string Logger::getLogLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "[DEBUG]  ";
        case LogLevel::INFO:    return "[INFO]   ";
        case LogLevel::WARNING: return "[WARNING]";
        case LogLevel::ERROR:   return "[ERROR]  ";
        case LogLevel::FATAL:   return "[FATAL]  ";
        case LogLevel::TODO:    return "[TODO]   ";
        default:                return "[UNKNOWN]";
    }
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::enableTimestamps(bool enable) {
    showTimestamps = enable;
}

void Logger::enableSourceInfo(bool enable) {
    showSourceInfo = enable;
}

void Logger::logInternal(LogLevel level, const std::string& message, const char* file, int line) {
    if (!initialized && !init()) {
        std::cerr << "Logger not initialized!" << std::endl;
        return;
    }

    std::stringstream logMessage;

    // Add timestamp if enabled
    if (showTimestamps) {
        logMessage << getCurrentTimestamp() << " ";
    }

    // Add log level
    logMessage << getLogLevelString(level) << " ";

    // Add source information if enabled and provided
    if (showSourceInfo && file != nullptr) {
        logMessage << "(" << file << ":" << line << ") ";
    }

    // Add the actual message
    logMessage << message;

    // Write to console
    std::cout << logMessage.str() << std::endl;

    // Write to file
    if (logFile.is_open()) {
        logFile << logMessage.str() << std::endl;
        logFile.flush(); // Ensure it's written immediately
    }
}

void Logger::debug(const std::string& message, const char* file, int line) {
    if (static_cast<int>(currentLevel) <= static_cast<int>(LogLevel::DEBUG)) {
        logInternal(LogLevel::DEBUG, message, file, line);
    }
}

void Logger::info(const std::string& message, const char* file, int line) {
    if (static_cast<int>(currentLevel) <= static_cast<int>(LogLevel::INFO)) {
        logInternal(LogLevel::INFO, message, file, line);
    }
}

void Logger::warning(const std::string& message, const char* file, int line) {
    if (static_cast<int>(currentLevel) <= static_cast<int>(LogLevel::WARNING)) {
        logInternal(LogLevel::WARNING, message, file, line);
    }
}

void Logger::error(const std::string& message, const char* file, int line) {
    if (static_cast<int>(currentLevel) <= static_cast<int>(LogLevel::ERROR)) {
        logInternal(LogLevel::ERROR, message, file, line);
    }
}

void Logger::fatal(const std::string& message, const char* file, int line) {
    if (static_cast<int>(currentLevel) <= static_cast<int>(LogLevel::FATAL)) {
        logInternal(LogLevel::FATAL, message, file, line);
    }
}

void Logger::todo(const std::string& message, const char* file, int line) {
    if (static_cast<int>(currentLevel) <= static_cast<int>(LogLevel::TODO)) {
        logInternal(LogLevel::TODO, message, file, line);
    }
}
