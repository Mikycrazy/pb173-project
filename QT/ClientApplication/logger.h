#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

class Logger
{
public:
    void Log(const std::string& log);

    static Logger* getLogger();
private:

    Logger() {}
    Logger( const Logger&){}
    Logger& operator=(const Logger& ){ return *this;}

    ~Logger() {}

    static const std::string mFileName;
    static std::ofstream mFile;
    static Logger* mpThis;

};

#endif // LOGGER_H
