#ifndef INCLUDED_CONFREAK_COMM
#define INCLUDED_CONFREAK_COMM
#include <curl_helper.h>
#include <string>
#include <vector>
#include <iostream>

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
private:
    EasyCurl d_ec;
    std::string d_baseUrl;

public:
    typedef std::vector<Pram> Args;
    Comm(const std::string& baseUrl):
    d_baseUrl(baseUrl)
    {};

    std::string loadAppData(const std::string& reqName, Args& args);
};

} // end of namespace confreak
#endif
