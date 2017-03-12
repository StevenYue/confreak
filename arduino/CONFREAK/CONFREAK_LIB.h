//Steeng: Sun Feb 26 15:48:42 EST 2017
#ifndef INCLUDED_CONFREAK
#define INCLUDED_CONFREAK
#include <Arduino.h> 

enum AppType {CONTROL_APP, MONITOR_APP};

const char PIPE = '|';
const char COMMA = ',';

struct App
{
    typedef     void*(*Action)(void*);
    
    String      appName;
    AppType     appType;
    long        data;
    Action      action;

    App(const String& name, AppType type, Action action=NULL):
        appName(name), appType(type), data(0.0), action(action)
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

    void readCommand()
    {
        if ( Serial.available() )
        {
            String command = Serial.readString();
            int appStrStartIndex = 0;
            for ( int i = 0, len = command.length(); i < len; ++i )
            {
                if ( COMMA == command.charAt(i) || i == (len - 1) )
                {
                    String appStr;
                    if ( i == ( len - 1 ) )
                    {
                        appStr = command.substring(appStrStartIndex);
                    }
                    else
                    {
                        appStr = command.substring(appStrStartIndex, i);
                    }
                    appStrStartIndex = i + 1;
                    String appData[3]; //appName|appType|appData
                    int dataStartIndex = 0;
                    int dataCount = 0;
                    for ( int j = 0, appStrLen = appStr.length(); j < appStrLen; ++j )
                    {
                        if ( PIPE == appStr.charAt(j) || j == (appStrLen - 1) )
                        {
                            if ( j == ( appStrLen - 1 ) )
                            {
                                appData[dataCount] = appStr.substring(dataStartIndex);
                            }
                            else
                            {
                                appData[dataCount] = appStr.substring(dataStartIndex, j);
                            }
                            dataStartIndex = j + 1;
                            ++dataCount;
                        }
                    }
                    if ( 3 == dataCount )
                    {
                        App* app = getAppByName(appData[0]);
                        if ( app )
                        {
                            long dataRead = appData[2].toInt();
                            if ( app->data != dataRead && NULL != app->action )
                            {
                                app->action(&dataRead);
                                app->data = dataRead;
                            }
                        }
                    }
                }
            }
        }
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
