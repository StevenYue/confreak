#include <confreak_schema.h>
#include <iostream>

namespace confreak {


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

    for ( auto i = keys.begin(); i != keys.end(); ++i )
    {
        std::wcout << data->Child(i->c_str())->Child(L"AppName")->AsString().c_str() << "   " 
            << data->Child(i->c_str())->Child(L"AppDesc")->AsString().c_str() << "   "
            << data->Child(i->c_str())->Child(L"AppType")->AsNumber() << "   "
            << data->Child(i->c_str())->Child(L"BoolData")->AsBool() << "   "
            << data->Child(i->c_str())->Child(L"NumericData")->AsNumber() << "   "
            << std::endl; 
    }
}


}; // end of namespace confreak

