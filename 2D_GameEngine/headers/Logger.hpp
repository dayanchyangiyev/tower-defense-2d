#ifndef Logger_hpp
#define Logger_hpp

#include <cstdio>
#include <string>

class Logger {
public:
    // Delete copy/move
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void init(const std::string& filename);
    void init(const char* filename);
    void log(const std::string& message);
    void log(const char* message);
    void close();

private:
    Logger() : logFile(nullptr) {}
    ~Logger() { close(); }
    
    std::FILE* logFile;
};

#endif /* Logger_hpp */
