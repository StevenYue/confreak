#ifndef INCLUDED_CONFREAK_SCHEMA
#define INCLUDED_CONFREAK_SCHEMA
#include <string>
#include <iostream>
#include <unordered_map>
#include <JSON.h>
#include <JSONValue.h>

namespace confreak {

struct ConfreakRt
{
    int             rc; //return code
    std::string     rs; //return string
};

class Application {
public:
    enum AppType  {CONTROL_APP, MONITOR_APP};
    static std::unordered_map<int, AppType>   AppTypeMap;
    static std::unordered_map<int, std::string>   AppTypeStrMap;
    Application(const std::string& appName, const std::string& appDesc, 
            AppType appType, bool boolData, double numData); 
    
    Application();

    friend std::ostream& operator<<(std::ostream& os, const Application& app)
    {
        os << "[AppName: " << app.d_appName << ", AppDesc: " << app.d_appDesc 
            << ", AppType: " << Application::AppTypeStrMap[app.d_appType] 
            << ", BoolData: " << app.d_boolData
            << ", NumericData: " << app.d_numericData << "]";
        return os;
    }

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

    friend std::ostream& operator<<(std::ostream& os, const ConfreakApps& apps)
    {
        os << "ConfreakApps: \n";
        for ( auto app = apps.d_apps.begin(); app != apps.d_apps.end(); ++app )
        {
           os << app->first << " : " << app->second << "\n";
        }
        return os;
    }
private:
    ConApp d_apps;

};


}; // end of namespace confreak
#endif
