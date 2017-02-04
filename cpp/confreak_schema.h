#ifndef INCLUDED_CONFREAK_SCHEMA
#define INCLUDED_CONFREAK_SCHEMA
#include <string>
#include <unordered_map>
#include <JSON.h>
#include <JSONValue.h>

namespace confreak {

class Application {
public:
    enum AppType  {CONTROL_APP, MONITOR_APP};
    Application(const std::string& appName, const std::string& appDesc, 
            AppType appType, bool boolData, double numData); 
    
    Application();

private:
    std::string d_appName; 
    std::string d_appDesc; 
    AppType     d_appType;
    bool        d_boolData;
    double      d_numericData;


};

class ConfreakApps {
public:
    typedef std::unordered_map<std::string, Application> ConApp;
    ConfreakApps(const std::string& jsonStr);

    ConApp& apps()
    {
        return d_apps;
    }
    
    Application& operator[](const std::string& key)
    {
        return d_apps[key];
    }

private:
    ConApp d_apps;

};


}; // end of namespace confreak
#endif
