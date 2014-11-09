#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <ctime>

class Logger
{
private:
    std::ofstream mFile;
public:
    Logger(const char* filename)
    {
        mFile.open(filename);

        if(mFile.is_open())
        {
            mFile << "Log file created" << std::endl;
        }
    }

    ~Logger();

    friend Logger &operator << (Logger &logger, const char* log)
    {
        time_t now = time(0);

        char* dt = ctime(&now);

        this->mFile << " - " << dt << ":\t";
        this->mFile << log << std::endl;
    }
};

#endif // LOGGER_H
