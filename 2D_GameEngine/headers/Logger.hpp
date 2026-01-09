#ifndef Logger_hpp
#define Logger_hpp

#include <iostream>
#include <fstream>
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
    void log(const std::string& message);
    void close();

private:
    Logger() = default;
    ~Logger() { close(); }
    
    std::ofstream logFile;
};

#endif /* Logger_hpp */
