#include <iostream>
#include <config_loader.h>
#include <vector>
#include <string>
#include <confreak_jail.h>

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
        confreak::Comm::Args args = {{std::string("email"), (*config)["LOGIN"]["email"].asString()},
            {std::string("password"), (*config)["LOGIN"]["password"].asString()}};
        confreak::ConfreakJail jail(baseUrl, serialPort, args);
        jail.start(); 
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
