#ifndef INCLUDED_CONFREAK_COMM
#define INCLUDED_CONFREAK_COMM
#include <curl_helper.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <confreak_schema.h>

extern "C" {
#include <serial_helper.h>
}

namespace confreak {

struct Pram
{
    std::string name;
    std::string value;
    Pram(const std::string& n, const std::string& v):
    name(n), value(v)
    {}
};

class Comm {
public:
    typedef std::vector<Pram> Args;
    Comm(const std::string& baseUrl, const std::string& serialPortName);
    ~Comm();

    ConfreakRt loadAppData(Args& args); 

    int serialWrite(const std::string& data);

    ConfreakRt serialRead();

private:
    EasyCurl d_ec;
    std::string d_baseUrl;
    int d_serialfd;

    ConfreakRt getWithArgs(const std::string& reqName, Args& args);

};

} // end of namespace confreak
#endif
