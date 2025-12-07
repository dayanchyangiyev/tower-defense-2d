#include "Logger.hpp"

std::ofstream Logger::logFile;

void Logger::init(const std::string& filename) {
    if (logFile.is_open()) {
        logFile.close();
    }
    // Open in truncate mode to overwrite
    logFile.open(filename, std::ios::out | std::ios::trunc);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

void Logger::log(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
    // Optional: Keep console output for critical errors? 
    // User requested to "clear console prints", so we suppress cout.
}

void Logger::close() {
    if (logFile.is_open()) {
        logFile.close();
    }
}
