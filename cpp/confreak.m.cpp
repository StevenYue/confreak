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
        Config::getInstance(argv[1]);
        confreak::ConfreakJail jail;
        jail.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
