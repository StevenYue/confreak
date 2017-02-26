#include <confreak_schema.h>
#include <iostream>
#include <sstream>

namespace confreak {

std::string w2s(const std::wstring& ws)
{
    return std::string(ws.begin(), ws.end());
}

std::unordered_map<int, Application::AppType> Application::AppTypeMap = 
    {{0, Application::CONTROL_APP}, {1, Application::MONITOR_APP}};

std::unordered_map<int, std::string> Application::AppTypeStrMap = 
    {{0, std::string("ControlApp")}, {1, std::string("MonitorApp")}};

Application::Application(const std::string& appName, const std::string& appDesc, 
    AppType appType, bool boolData, const std::string& numData):
    d_appName(appName), d_appDesc(appDesc), d_appType(appType), 
    d_boolData(boolData), d_numericData(numData)
{}

Application::Application()
{}

void Application::updateAppData(const Application& o)
{
    if ( o.d_appType == Application::CONTROL_APP )
    {
        this->d_boolData = o.d_boolData;
    }
    else if ( o.d_appType == Application::MONITOR_APP )
    {
        this->d_numericData = o.d_numericData; 
    }
    else
    {
        this->d_numericData = o.d_numericData;
        this->d_boolData = o.d_boolData; 
    }
}

std::string& Application::appName()
{
    return d_appName;
}
std::string& Application::appDesc()
{
    return d_appDesc;
}
Application::AppType& Application::appType()
{
    return d_appType;
}
bool&        Application::boolData()
{
    return d_boolData;
}
std::string& Application::numericData()
{
    return d_numericData;
}

ConfreakApps::ConfreakApps(const std::string& jsonStr, Application::AppType type)
{
    JSONValue *data = JSON::Parse(jsonStr.c_str());
    if ( !data )
    {
        std::ostringstream os;
        os << "Error initializ ConfreakApps parsing json, jsonstr: " << jsonStr;
        throw std::runtime_error(os.str());
    }
    std::vector<std::wstring> keys = data->ObjectKeys();
    for ( auto key = keys.begin(); key != keys.end(); ++key )
    {
        JSONValue *row = data->Child(key->c_str());
        if ( !row )
        {
            std::ostringstream os;
            os << "Error initializ ConfreakApps needs vector format, jsonstr: " << jsonStr;
            throw std::runtime_error(os.str());
        }
        std::string appName = saveJSON(row->Child(L"AppName"), std::string(""));
        std::string appDesc = saveJSON(row->Child(L"AppDesc"), std::string(""));
        Application::AppType appType = Application::AppTypeMap[saveJSON(row->Child(L"AppType"), double(-1))];
        if ( type == appType || type == Application::ALL_APP )
        {
            bool boolData       = saveJSON(row->Child(L"BoolData"), false);
            std::string numData = saveJSON(row->Child(L"NumericData"), std::string(""));
            d_apps[appName] = Application(appName, appDesc, appType, boolData, numData);
        }
    }
}

ConfreakApps::ConfreakApps()
{}

bool        ConfreakApps::saveJSON(const JSONValue* json, bool defaultValue)
{
    return json ? json->AsBool() : defaultValue;
}
double      ConfreakApps::saveJSON(const JSONValue* json, double defaultValue)
{
    return json ? json->AsNumber() : defaultValue;
}
std::string ConfreakApps::saveJSON(const JSONValue* json, std::string&& defaultValue)
{
    return json ? w2s(json->AsString()) : defaultValue;
}

}; // end of namespace confreak

