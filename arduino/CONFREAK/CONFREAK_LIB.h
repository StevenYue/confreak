//Steeng: Sun Feb 26 15:48:42 EST 2017
#ifndef INCLUDED_CONFREAK
#define INCLUDED_CONFREAK
#include <Arduino.h> 

enum AppType {CONTROL_APP, MONITOR_APP};

const String PIPE("|");
const String COMMA(",");

struct App
{
    String      appName;
    AppType     appType;
    long        data;
   
    App(const String& name, AppType type):
        appName(name), appType(type), data(0.0)
    {}

    friend bool operator==(const App& l, const App& r)
    {
        return l.appName == r.appName && l.appType == r.appType && l.data == r.data;
    }
    
    friend bool operator!=(const App& l, const App& r)
    {
        return !(l==r);
    }

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
    {
        if (!Serial)
        {
            Serial.begin(9600);
        }
    }

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

    App* updateAppData(const String& name, long data)
    {
        App* app = getAppByName(name);
        if ( app )
        {
            app->data = data;
        }
        return app;
    }

    void sendUpdate()
    {
        Serial.write(toString().c_str());
    }

    String toString()
    {
        String res;
        res.reserve(d_size << 5);
        for ( int i = 0; i < d_size; ++i )
        {
            if ( i != 0 )
            {
                res += COMMA;
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
