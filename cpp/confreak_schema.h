#ifndef INCLUDED_CONFREAK_SCHEMA
#define INCLUDED_CONFREAK_SCHEMA
#include <string>
#include <iostream>
#include <unordered_map>
#include <JSON.h>
#include <JSONValue.h>
#include <logger.h>

namespace confreak {

struct ConfreakRt
{
    int             rc; //return code
    std::string     rs; //return string
};


class Application {
public:
    enum AppType  {CONTROL_APP, MONITOR_APP, ALL_APP};
    static std::unordered_map<int, AppType>   AppTypeMap;
    static std::unordered_map<int, std::string>   AppTypeStrMap;
    Application(const std::string& appName, const std::string& appDesc, 
            AppType appType, bool boolData, const std::string&  numData); 
    
    Application();

    std::string&    appName(); 
    std::string&    appDesc(); 
    AppType&        appType();
    bool&           boolData();
    std::string&    numericData();

    void updateAppData(const Application& o);

    friend std::ostream& operator<<(std::ostream& os, const Application& app)
    {
        os << "[AppName: " << app.d_appName << ", AppDesc: " << app.d_appDesc 
            << ", AppType: " << Application::AppTypeStrMap[app.d_appType] 
            << ", BoolData: " << app.d_boolData
            << ", NumericData: " << app.d_numericData << "]";
        return os;
    }

    friend bool operator==(const Application& l, const Application& r)
    {
        if ( l.d_appType == Application::CONTROL_APP )
        {
            return l.d_appName == r.d_appName && l.d_appType == r.d_appType && l.d_boolData == r.d_boolData;
        }
        else if ( l.d_appType == Application::MONITOR_APP )
        {
            return l.d_appName == r.d_appName && l.d_appType == r.d_appType && l.d_numericData == r.d_numericData; 
        }
        else
        {
            return l.d_appName == r.d_appName && l.d_appType == r.d_appType
                && l.d_numericData == r.d_numericData && l.d_boolData == r.d_boolData; 
        }
    }

    friend bool operator!=(const Application& l, const Application& r)
    {
        return !(l==r);
    }

private:
    std::string d_appName; 
    std::string d_appDesc; 
    AppType     d_appType;
    bool        d_boolData;
    std::string d_numericData;
    
};

class ConfreakApps {
public:
    typedef std::unordered_map<std::string, Application> ConApp;
    ConfreakApps(const std::string& jsonStr, Application::AppType appType = Application::ALL_APP);
    ConfreakApps();

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
    
    bool        saveJSON(const JSONValue* json, bool defaultValue);
    double      saveJSON(const JSONValue* json, double defaultValue);
    std::string saveJSON(const JSONValue* json, std::string&& defaultValue);
};


}; // end of namespace confreak
#endif
