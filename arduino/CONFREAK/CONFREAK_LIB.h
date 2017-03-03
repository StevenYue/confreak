//Steeng: Sun Feb 26 15:48:42 EST 2017
#ifndef INCLUDED_CONFREAK
#define INCLUDED_CONFREAK
#include <Arduino.h> 

enum AppType {CONTROL_APP, MONITOR_APP};

const String PIPE("|");
const String COMMA(",");

class App
{
public:
    String      appName;
    AppType     appType;
    long        data;
   
    App(const String& name, AppType type):
        appName(name), appType(type), data(0.0)
    {}

    String toString()
    {
        String res;
        res.reserve(32);
        res = appName;
        res += PIPE;
        res += String(appType);
        res += PIPE;
        res += String(data);
        return res;
    }
};

class Confreak
{
public:
    Confreak(){}

    Confreak(App* apps, int size):
        d_apps(apps), d_size(size)
    {}

    App* apps()
    {
        return d_apps;
    }

    int& size()
    {
        return d_size;
    }

    App* getAppByName(const String& name)
    {
        for ( int i = 0; i < d_size; ++i )
        {
            if ( name == d_apps[i].appName )
            {
                return &d_apps[i];
            }
        }
        return NULL;
    }

    String toString()
    {
        String res;
        res.reserve(d_size >> 5);
        for ( int i = 0; i < d_size; ++i )
        {
            if ( i != 0 )
            {
                res += PIPE;
            }
            res += d_apps[i].toString();
        }
        return res;
    }

private:
    App*    d_apps;
    int     d_size;
};

#endif
//------ end of CONFREAK_LIB.h ------
