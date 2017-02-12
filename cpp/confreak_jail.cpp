//Steeng created at Sat Feb 11 23:52:45 EST 2017
#include <confreak_jail.h>
namespace confreak { 

ConfreakJail::ConfreakJail()
{
    Warden wardenControl(Application::CONTROL_APP, controlDuty);
    Warden wardenMonitor(Application::MONITOR_APP, monitorDuty);
    pthread_t threadControl, threadMonitor;
    d_wardens.push_back(std::make_pair(wardenControl, threadControl));
    d_wardens.push_back(std::make_pair(wardenMonitor, threadMonitor));
    std::cout << "Jail load up" << std::endl;
}

ConfreakJail::~ConfreakJail()
{
    for ( auto it = d_wardens.begin(); it != d_wardens.end(); ++it )
    {
        int rc = pthread_join(it->second, NULL);  
        assert(!rc);
    }
}

void ConfreakJail::start()
{
    
    for ( auto it = d_wardens.begin(); it != d_wardens.end(); ++it )
    {
        int rc = pthread_create(&it->second, NULL, it->first.duty(), &it->first); 
        assert(!rc);
    }
    std::cout << "Jail Open" << std::endl;
}

}  //end of namespace confreak
