#ifndef Logger_hpp
#define Logger_hpp

#include <iostream>
#include <fstream>
#include <string>

class Logger {
public:
    static void init(const std::string& filename);
    static void log(const std::string& message);
    static void close();

private:
    static std::ofstream logFile;
};

#endif /* Logger_hpp */
