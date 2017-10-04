#include <confreak_schema.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <fstream>

namespace confreak {

std::unordered_map<int, Application::AppType> Application::AppTypeMap = 
    {{0, Application::CONTROL_APP}, {1, Application::MONITOR_APP}};

std::unordered_map<int, std::string> Application::AppTypeStrMap = 
    {{0, "ControlApp"}, {1, "MonitorApp"}};

std::unordered_map<std::string, Application::AppType>   Application::AppStrTypeMap = 
    {{"0", Application::CONTROL_APP}, {"1", Application::MONITOR_APP}};

Application::Application(
        const std::string&  appName, 
        const std::string&  appDesc, 
        AppType             appType, 
        double              numData):
d_appName(appName), d_appDesc(appDesc), d_appType(appType), d_data(numData)
{}

Application::Application(
    const std::string&  appName, 
    const std::string&  appDesc, 
    AppType             appType, 
    const std::string&  d): 
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
    if ( d_appType == Application::CONTROL_APP )
    {
        if ( d_data > 0 )
        {
            return "1";
        }
        else
        {
            return "0";
        }
    }
    else
    {
        return std::to_string(d_data);
    }
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
    Json::Reader reader;
    Json::Value obj;
    bool isOk = reader.parse(jsonStr, obj);
    if ( !isOk && obj.isArray() )
    {
        std::ostringstream os;
        os << "Error initializ ConfreakApps parsing json, jsonstr: " << jsonStr 
            << "\n isOK:" << isOk << ", isArray:" << obj.isArray();
        throw std::runtime_error(os.str());
    }
    for ( unsigned int i = 0; i < obj.size(); ++i )
    {
        Json::Value row = obj[i];
        if ( !row.isObject() )
        {
            std::ostringstream os;
            os << "Expect Json Object:" << row;
            throw std::runtime_error(os.str());
        }
        std::string appName = row["AppName"].asString();
        std::string appDesc = row["AppDesc"].asString();
        Application::AppType appType = Application::AppTypeMap[row["AppType"].asInt()];
        if ( type == appType || type == Application::ALL_APP )
        {
            double data = row["Data"].asDouble();
            d_apps[appName] = Application(appName, appDesc, appType, data);
        }
    }
}

ConfreakApps::ConfreakApps()
{}

}; // end of namespace confreak

