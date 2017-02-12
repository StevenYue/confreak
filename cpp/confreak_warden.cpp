//Steeng created at Sat Feb 11 19:27:05 EST 2017
#include <confreak_warden.h>
#include <config_loader.h>
namespace confreak { 

Warden::Warden(Application::AppType appType, Duty duty)
{
    d_duty = duty;
    Config* config = Config::getInstance();
    d_comm = confreak::Comm((*config)["COMM"]["baseurl"].asString(), 
            (*config)["COMM"]["serial"].asString(), 
            {{std::string("email"), (*config)["LOGIN"]["email"].asString()},
            {std::string("password"), (*config)["LOGIN"]["password"].asString()}});

    confreak::ConfreakRt cr = d_comm.loadAppData();
    assert ( cr.rc == 0 );
    d_apps = confreak::ConfreakApps(cr.rs, appType);
    std::cout << "Warden " << Application::AppTypeStrMap[appType] << " badge in" << std::endl;
}

Warden::~Warden()
{}

Warden::Duty& Warden::duty()
{
    return d_duty;
}

void* monitorDuty(void* data)
{
    return NULL;    
}

void* controlDuty(void* data)
{
    return NULL;    
}

}  //end of namespace confreak
