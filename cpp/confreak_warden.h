//Steeng created at Sat Feb 11 19:27:05 EST 2017
#ifndef INCLUDED_CONFREAK_WARDEN
#define INCLUDED_CONFREAK_WARDEN

#include <string>
#include <unordered_map>
#include <confreak_schema.h>
#include <confreak_comm.h>
#include <cassert>

namespace confreak{ 

//Omnipotent warden oversees all freaks
class Warden {
public:
    typedef void*(*Duty)(void*);
    Warden(Application::AppType appType, Duty duty);

    ~Warden();

    friend std::ostream& operator<<(std::ostream& os, const Warden& warden)
    {
        os << "Warden handles: " << warden.d_apps;
        return os;
    }
    
    Duty&           duty();

private:
    Duty            d_duty;
    Comm            d_comm;
    ConfreakApps    d_apps;
    
};

void* monitorDuty(void* data);
void* controlDuty(void* data);

}  //end of namespace confreak
#endif