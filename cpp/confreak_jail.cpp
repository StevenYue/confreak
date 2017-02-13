//Steeng created at Sat Feb 11 23:52:45 EST 2017
#include <confreak_jail.h>
namespace confreak { 

ConfreakJail::ConfreakJail()
{
    std::cout << "Empty Jail load up" << std::endl;
}

ConfreakJail::ConfreakJail(const std::string& baseUrl, const std::string& serialPort, const Comm::Args& args):
d_controlWarden(Application::CONTROL_APP, controlDuty, baseUrl, serialPort, args),
d_monitorWarden(Application::MONITOR_APP, monitorDuty, baseUrl, serialPort, args)
{
    std::cout << "Jail loaded up" << std::endl;
}

ConfreakJail::~ConfreakJail()
{
    int rc = pthread_join(d_monitorThread, NULL);  
    assert(!rc);
    rc = pthread_join(d_controlThread, NULL);  
    assert(!rc);
}

void ConfreakJail::start()
{
    //int rc = pthread_create(&d_controlThread, NULL, d_controlWarden.duty(), &d_controlWarden); 
    //assert(!rc);
    int rc = pthread_create(&d_monitorThread, NULL, d_monitorWarden.duty(), &d_monitorWarden); 
    assert(!rc);
    std::cout << "Jail Open" << std::endl;
}

}  //end of namespace confreak
