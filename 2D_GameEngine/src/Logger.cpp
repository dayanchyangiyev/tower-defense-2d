#include "Logger.hpp"

#include <cstring>

void Logger::init(const std::string& filename) {
    init(filename.c_str());
}

void Logger::init(const char* filename) {
    if (logFile) {
        std::fclose(logFile);
        logFile = nullptr;
    }
    if (!filename) {
        std::fprintf(stderr, "Failed to open log file: (null)\n");
        return;
    }
    // Open in truncate mode to overwrite
    logFile = std::fopen(filename, "w");
    if (!logFile) {
        std::fprintf(stderr, "Failed to open log file: %s\n", filename);
    }
}

void Logger::log(const std::string& message) {
    log(message.c_str());
}

void Logger::log(const char* message) {
    if (!message) {
        message = "(null)";
    }
    if (logFile) {
        std::fwrite(message, 1, std::strlen(message), logFile);
        std::fwrite("\n", 1, 1, logFile);
        std::fflush(logFile);
    } else {
        std::fprintf(stdout, "[Console Fallback]: %s\n", message);
        std::fflush(stdout);
    }
}

void Logger::close() {
    if (logFile) {
        std::fclose(logFile);
        logFile = nullptr;
    }
}
