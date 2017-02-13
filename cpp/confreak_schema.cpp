#include <confreak_schema.h>

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
    AppType appType, bool boolData, double numData):
    d_appName(appName), d_appDesc(appDesc), d_appType(appType), 
    d_boolData(boolData), d_numericData(numData)
{}

Application::Application()
{}

ConfreakApps::ConfreakApps(const std::string& jsonStr)
{
    JSONValue *data = JSON::Parse(jsonStr.c_str());
    std::vector<std::wstring> keys = data->ObjectKeys();
    for ( auto key = keys.begin(); key != keys.end(); ++key )
    {
        JSONValue *row = data->Child(key->c_str());
        std::string appName = w2s(row->Child(L"AppName")->AsString());
        std::string appDesc = w2s(row->Child(L"AppDesc")->AsString());
        Application::AppType appType = Application::AppTypeMap[row->Child(L"AppType")->AsNumber()];
        bool boolData = row->Child(L"BoolData")->AsBool();
        bool numData = row->Child(L"NumericData")->AsNumber();

        d_apps[appName] = Application(appName, appDesc, appType, boolData, numData);
    }
}

ConfreakApps::ConfreakApps(const std::string& jsonStr, Application::AppType type)
{
    JSONValue *data = JSON::Parse(jsonStr.c_str());
    std::vector<std::wstring> keys = data->ObjectKeys();
    for ( auto key = keys.begin(); key != keys.end(); ++key )
    {
        JSONValue *row = data->Child(key->c_str());
        std::string appName = w2s(row->Child(L"AppName")->AsString());
        std::string appDesc = w2s(row->Child(L"AppDesc")->AsString());
        Application::AppType appType = Application::AppTypeMap[row->Child(L"AppType")->AsNumber()];
        if ( type == appType )
        {
            bool boolData = row->Child(L"BoolData")->AsBool();
            bool numData = row->Child(L"NumericData")->AsNumber();
            d_apps[appName] = Application(appName, appDesc, appType, boolData, numData);
        }
    }
}

ConfreakApps::ConfreakApps()
{}

AppTracker::AppTracker(const std::string& jsonStr)
{
    JSONValue *json = JSON::Parse(jsonStr.c_str());
    if ( json == NULL )
    {
        return;
    }
    std::vector<std::wstring> keys = json->ObjectKeys();
    for ( auto key = keys.begin(); key != keys.end(); ++key )
    {
        std::cout << "key:" << w2s(*key) << std::endl;
        std::cout << "val:" << w2s(json->Child(key->c_str())->AsString()) << std::endl;
        std::string k(w2s(*key).c_str());
        std::string v(w2s(json->Child(key->c_str())->AsString()).c_str());
        std::cout << "k:" << k << std::endl;
        std::cout << "v:" << v << std::endl;
        info.insert({k,v});
    }
    std::string t("test");
    info[t] = t;
    for (auto it = info.begin(); it != info.end(); ++it )
    {
        std::cout << it->first << "--" << it->second << std::endl; 
    }
    std::cout << "11:" << info["appname"] << std::endl;
    std::cout << "22:" << info["data"] << std::endl;
    std::cout << "33:" << info["test"] << std::endl;
}

}; // end of namespace confreak

