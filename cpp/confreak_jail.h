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
#include <string>
namespace confreak { 

class ConfreakJail {
public:
    ConfreakJail(); 
    ConfreakJail(const std::string& url, const std::string& serial, const Comm::Args& args); 
    
    ~ConfreakJail(); 
  
    void start();

private:
    Warden          d_controlWarden;
    Warden          d_monitorWarden;
    pthread_t       d_controlThread;
    pthread_t       d_monitorThread;

};

}  //end of namespace confreak
#endif
