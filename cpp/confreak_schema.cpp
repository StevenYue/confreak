#include <confreak_schema.h>

namespace confreak {

std::string w2s(const std::wstring& ws)
{
    return std::string((const char*)&ws[0], sizeof(wchar_t)/sizeof(char)*ws.size());
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


}; // end of namespace confreak

