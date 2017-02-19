#ifndef INCLUDED_CONFREAK_COMM
#define INCLUDED_CONFREAK_COMM
#include <curl_helper.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <confreak_schema.h>
#include <logger.h>

extern "C" {
#include <serial_helper.h>
}

namespace confreak {

class Comm {
public:
    typedef std::unordered_map<std::string, std::string> Args;
    Comm(const std::string& baseUrl, const std::string& serialPortName, const Args& args);
    Comm();
    ~Comm();

    ConfreakRt loadAppData(const Args& args); 
    
    ConfreakRt loadAppData(); 
    
    ConfreakRt updateAppData(const std::string& appName, const std::string& data, Application::AppType type);

    int serialWrite(const std::string& data);

    ConfreakRt serialRead();
    
    Args& args();

private:
    EasyCurl        d_ec;
    std::string     d_baseUrl;
    int             d_serialfd;
    Args            d_args;

    ConfreakRt getWithArgs(const std::string& reqName, const Args& args);
};

inline Comm::Args& Comm::args()
{
    return d_args;
}

} // end of namespace confreak
#endif
