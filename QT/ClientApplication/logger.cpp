#include "logger.h"

Logger* Logger::mpThis = NULL;
std::ofstream Logger::mFile;

Logger* Logger::getLogger()
{
    if(mpThis == NULL){
        mpThis = new Logger();
        mFile.open("m_sFileName.txt", std::ios::out | std::ios::app );

        std::cout << "File created" << std::endl;
    }

    return mpThis;
}

void Logger::Log(const std::string& log)
{
    time_t now = time(0);

    char* dt = ctime(&now);

    mFile << log << std::endl;
}
