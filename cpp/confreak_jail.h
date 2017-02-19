//Steeng created at Sat Feb 11 23:52:45 EST 2017
#ifndef INCLUDED_CONFREAK_JAIL
#define INCLUDED_CONFREAK_JAIL
extern "C" {
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
}

#include <confreak_warden.h>
#include <confreak_schema.h>
#include <confreak_comm.h>
#include <vector>
#include <string>
#include <logger.h>
namespace confreak { 

class ConfreakJail {
public:
    typedef std::vector<std::pair<Application::AppType, Warden::Duty> > JOBS;
    ConfreakJail(); 
    ConfreakJail(const std::string& url, const std::string& serial, const Comm::Args& args, const JOBS& jobs); 
    
    ~ConfreakJail(); 
  
    void start();

private:
    std::vector<Warden>         d_wardens;
    std::vector<pthread_t>      d_jobs;

};

}  //end of namespace confreak
#endif
