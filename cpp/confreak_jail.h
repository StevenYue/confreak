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
#include <vector>
namespace confreak { 

class ConfreakJail {
public:
    ConfreakJail(); 
    
    ~ConfreakJail(); 
  
    void start();

private:
    std::vector<std::pair<Warden, pthread_t> >  d_wardens;

};

}  //end of namespace confreak
#endif
