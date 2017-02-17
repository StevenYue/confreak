#include <iostream>
#include <config_loader.h>
#include <vector>
#include <string>
#include <confreak_jail.h>

using namespace confreak;

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        std::cout << "Confreak Usage: confreak.tsk confreak.cfg" << std::endl;
        exit(1);
    }
    try
    {
        Config* config = Config::getInstance(argv[1]);
        std::string baseUrl = (*config)["COMM"]["baseurl"].asString();
        std::string serialPort = (*config)["COMM"]["serial"].asString();
        Comm::Args args = {{std::string("email"), (*config)["LOGIN"]["email"].asString()},
            {std::string("password"), (*config)["LOGIN"]["password"].asString()}};
        ConfreakJail::JOBS jobs; 
        if ( (*config)["APPS"]["monitorapp"].asBool() )
        {
            jobs.push_back(std::make_pair(Application::MONITOR_APP, monitorDuty));
        }
        if ( (*config)["APPS"]["controlapp"].asBool() )
        {
            jobs.push_back(std::make_pair(Application::CONTROL_APP, controlDuty));
        }

        ConfreakJail jail(baseUrl, serialPort, args, jobs);
        jail.start(); 
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
