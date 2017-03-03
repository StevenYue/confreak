#include <confreak_schema.h>
#include <iostream>
#include <sstream>
#include <memory>

namespace confreak {

std::string w2s(const std::wstring& ws)
{
    return std::string(ws.begin(), ws.end());
}

std::unordered_map<int, Application::AppType> Application::AppTypeMap = 
    {{0, Application::CONTROL_APP}, {1, Application::MONITOR_APP}};

std::unordered_map<int, std::string> Application::AppTypeStrMap = 
    {{0, "ControlApp"}, {1, "MonitorApp"}};

std::unordered_map<std::string, Application::AppType>   Application::AppStrTypeMap = 
    {{"0", Application::CONTROL_APP}, {"1", Application::MONITOR_APP}};

Application::Application(const std::string& appName, const std::string& appDesc, AppType appType, double numData):
d_appName(appName), d_appDesc(appDesc), d_appType(appType), d_data(numData)
{}

Application::Application(
    const std::string& appName, 
    const std::string& appDesc, 
    AppType appType, 
    const std::string& d): 
d_appName(appName), d_appDesc(appDesc), d_appType(appType)
{
    setData(d);
}

Application::Application()
{}

void Application::updateAppData(const Application& o)
{
    this->d_data = o.d_data; 
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
double&     Application::data()
{
    return d_data;
}

int Application::setAppType(const std::string& tStr)
{
    if ( Application::AppStrTypeMap.find(tStr) != Application::AppStrTypeMap.end() )
    {
        d_appType = Application::AppStrTypeMap[tStr];
        return 0;
    }
    else
    {
        return -1;
    }
}

std::string Application::appTypeAsString() const
{
    return std::to_string(d_appType);
}

std::string Application::dataAsString() const
{
    return std::to_string(d_data);
}

int Application::setData(const std::string& dStr)
{
    try 
    {
        d_data = std::stod(dStr);
        return 0;
    }
    catch (const std::invalid_argument& ia)
    {
        d_data = 0;
        return -1;
    }
}

ConfreakApps::ConfreakApps(const std::string& jsonStr, Application::AppType type)
{
    std::unique_ptr<JSONValue> data(JSON::Parse(jsonStr.c_str()));
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
            double data = saveJSON(row->Child(L"Data"),double(0));
            d_apps[appName] = Application(appName, appDesc, appType, data);
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

