#include <iostream>
#include <config_loader.h>

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
        std::cout << "email:    " << (*config)["LOGIN"]["email"].asString() << std::endl;
        std::cout << "password: " << (*config)["LOGIN"]["password"].asString() << std::endl;
        std::cout << "password: " << (*config)["LOGIN"]["password"].asInt() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
