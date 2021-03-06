//Steeng created at Sat Feb 11 23:52:45 EST 2017
#include <confreak_jail.h>
namespace confreak { 

ConfreakJail::ConfreakJail()
{
    LOG_INFO << "Empty Jail load up" << LOG_END;
}

ConfreakJail::ConfreakJail(const std::string& baseUrl, 
        const std::string& serialPort, 
        const Comm::Args& args, 
        const JOBS& jobs)
{
    for ( auto it = jobs.begin(); it != jobs.end(); ++ it )
    {
        d_wardens.push_back(Warden(it->first, it->second, baseUrl, serialPort, args));
    }
    LOG_INFO << "Jail loaded up" << LOG_END;
}

ConfreakJail::~ConfreakJail()
{
    for ( auto it = d_jobs.begin(); it != d_jobs.end(); ++it )
    {
        int rc = pthread_join(*it, NULL);  
        assert(!rc);
    }
}

void ConfreakJail::start()
{
    for ( auto it = d_wardens.begin(); it != d_wardens.end(); ++it )
    {
        pthread_t thread;
        int rc = pthread_create(&thread, NULL, it->duty(), &(*it)); 
        assert(!rc);
        d_jobs.push_back(thread);
    }
    LOG_INFO << "Jail Open" << LOG_END;
}

}  //end of namespace confreak
