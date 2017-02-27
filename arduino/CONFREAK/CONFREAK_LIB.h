//Steeng: Sun Feb 26 15:48:42 EST 2017
#ifndef INCLUDED_CONFREAK
#define INCLUDED_CONFREAK
#include <Arduino.h> 
#include <VECTOR_LIB.h>

enum AppType {CONTROL_APP, MONITOR_APP};

class App
{
public:
    String      appName;
    AppType     appType;
    double      numData;
    bool        boolData;
   
    App::App(const String& name, AppType type):
        appName(name), appType(type), numData(0), boolData(false)
    {}

    String toJsonString()
    {
        String res = String("{\"AppName\":\"");
        res += appName;
        res += String("\", \"AppType\":");
        res += String(appType);
        if ( CONTROL_APP == appType )
        {
            res += String(", \"BoolData\":");
            res += String(boolData);
        }
        else if ( MONITOR_APP == appType )
        {
            res += String(", \"NumericData\":");
            res += String(numData);
        }
        res += String("}");
        return res;
    }
};

class Confreak
{
public:
    Confreak(){}

    Confreak(App* apps, int size)
    {
        for ( int i = 0; i < size; ++i )
        {
            d_apps.push_back(apps[i]);
        }
    }

    Vector<App>& apps()
    {
        return d_apps;
    }

    App* getAppByName(const String& name)
    {
        for ( int i = 0; i < d_apps.size(); ++i )
        {
            if ( name == d_apps[i].appName )
            {
                return &d_apps[i];
            }
        }
        return NULL;
    }

    String toJsonString()
    {
        String res = String("{");
        for ( int i = 0; i < d_apps.size(); ++i )
        {
            if ( i != 0 )
            {
                res += String(",");
            }
            res += String("\"");
            res += d_apps[i].appName;
            res += String("\":");
            res += d_apps[i].toJsonString();
        }
        res += String("}");
        return res;
    }

private:
    Vector<App> d_apps;
};

#endif
//------ end of CONFREAK_LIB.h ------
