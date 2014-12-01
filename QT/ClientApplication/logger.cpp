#include "logger.h"

Logger* Logger::mpThis = NULL;
std::ofstream Logger::mFile;

Logger::~Logger()
{

}

Logger* Logger::getLogger()
{
    if(mpThis == NULL){
        mpThis = new Logger();
        mFile.open("log.txt", std::ios::out | std::ios::app );
    }
    return mpThis;
}

void Logger::Log(const std::string& log)
{
    time_t now = time(0);

    char* dt = ctime(&now);

    mFile << " - " << dt << "\t";
    mFile << log << std::endl;

    qDebug() << log.c_str();

}
