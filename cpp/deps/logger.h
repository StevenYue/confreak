//Steeng created at Sat Feb 18 08:23:37 EST 2017
#ifndef INCLUDED_LOGGER
#define INCLUDED_LOGGER
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <ctime>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_INFO    std::cout << dateTimeStr() << " " << __FILENAME__ << ":" << __LINE__ << " "
#define LOG_ERROR   std::cerr << dateTimeStr() << " " << __FILENAME__ << ":" << __LINE__ << " "
#define LOG_END     std::endl

//Output time in MM/DD/YYYY HH/MM/SS/UU  e.g. 02/18/17 11:33:52:536647
static std::string dateTimeStr()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    char dtBuf[100];
    std::strftime(dtBuf, sizeof(dtBuf), "%D %T",std::localtime(&tv.tv_sec));
    std::ostringstream os;
    os << dtBuf << ":" << tv.tv_usec;
    return os.str();
}

static std::string date(const std::string& format)
{
    std::time_t secs = std::time(nullptr);
    char dtBuf[100];
    std::strftime(dtBuf, sizeof(dtBuf), format.c_str(),std::localtime(&secs));
    return std::string(dtBuf);
}

class Logger{
public:
    static Logger* getInstance(const std::string& dir, const std::string& fileName)
    {
        if ( !d_logger )
        {
            d_logger = new Logger(dir, fileName);
        }
        return d_logger;
    }
    
    ~Logger()
    {
        LOG_INFO << "Logger closed" << LOG_END;
    }

private:
    Logger(const std::string& dir, const std::string& fileName)
    {
        std::string filePath;
        if ( dir != "" )
        {
            filePath = "./" + dir + "/" + fileName + "." + date("%Y.%m.%d");
        }
        else
        {
            filePath = fileName + "." + date("%Y.%m.%d");
        }
        d_of.open(filePath.c_str(), std::ofstream::out | std::ofstream::app);
        d_oldCoutBuf = std::cout.rdbuf();
        d_oldCerrBuf = std::cerr.rdbuf();
        std::cout.rdbuf(d_of.rdbuf());
        std::cerr.rdbuf(d_of.rdbuf());
        LOG_INFO << "Logger setup at file:" << filePath << LOG_END;
    }

    std::ofstream       d_of;
    std::streambuf*     d_oldCoutBuf;
    std::streambuf*     d_oldCerrBuf;
    static Logger*      d_logger;
};


#endif
