#include <iostream>
#include <config_loader.h>
#include <confreak_comm.h>
#include <vector>
#include <string>
#include <confreak_schema.h>

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
        std::cout << "baseurl: " << (*config)["COMM"]["baseurl"].asString() << std::endl;
        confreak::Comm comm = confreak::Comm((*config)["COMM"]["baseurl"].asString(), 
                (*config)["COMM"]["serial"].asString());
        confreak::Comm::Args args = {confreak::Pram("email", (*config)["LOGIN"]["email"].asString()),
            confreak::Pram("password", (*config)["LOGIN"]["password"].asString())};
        confreak::ConfreakRt cr = comm.loadAppData(args);

        if ( cr.rc == 0 )
        {
            confreak::ConfreakApps test(cr.rs);
            std::cout << test << std::endl;
        }
        else
        {
            std::cerr << cr.rc << std::endl;
            std::cerr << cr.rs << std::endl;
        }
        std::cout << "serial write:" << comm.serialWrite("Steve") << std::endl;
        //cr = comm.serialRead();
        //std::cout << "Serial Read:" << cr.rc << "-" << cr.rs << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception Caught: " << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
