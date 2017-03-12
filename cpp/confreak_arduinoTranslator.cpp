//Steeng created at Mon Feb 27 22:12:58 EST 2017
#include <confreak_arduinoTranslator.h>
#include <sstream>

namespace confreak { 
namespace arduinoTranslator{

const std::string COMMA(",");
const std::string PIPE("|");

std::string toArduino(Application& app)
{
    return app.appName() + PIPE + std::to_string(app.appType()) + PIPE + app.dataAsString();
}

std::string toArduino(ConfreakApps& apps)
{
    std::string res("");
    for ( auto it = apps.apps().begin(); it != apps.apps().end(); ++it )
    {
        if ( res != "" )
        {
            res += COMMA;
        }
        res += toArduino(it->second); 
    }
    return res;
}

int toApplication(const std::string& arduinoStr, Application& app)
{
    int rc;
    std::istringstream is(arduinoStr);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(is, token, PIPE.at(0)))
    {
        tokens.push_back(token); 
    }
    //An application data will always comprised from 3 sections
    //Name|Type|Data
    if ( tokens.size() == 3 )
    {
        app.appName() = tokens[0];
        rc = app.setAppType(tokens[1]);
        rc = app.setData(tokens[2]);
    }
    else
    {
        rc = -1;
    }
    return rc;
}

int toConfreakApps(const std::string& arduinoStr, ConfreakApps& apps)
{
    int rc;
    std::istringstream is(arduinoStr);
    std::string token;
    while (std::getline(is, token, COMMA.at(0)))
    {
        Application app;
        rc = toApplication(token, app);
        if ( !rc )
        {
            apps.apps()[app.appName()] = app;
        }
    }
    return rc;
}

}   //end of namespace arduinoTranslator
}   //end of namespace confreak
