//Steeng: Sun Feb 26 15:48:42 EST 2017
#include "CONFREAK_LIB.h"

//Define the action function, should also takes in a void point and return a void pointer
//only long will be passed into the function, just cast it to long
void* lightUp13(void* arg)
{
    long* data = (long*)arg;
    digitalWrite(13, *data); 
}

void* lightUp7(void* arg)
{
    long* data = (long*)arg;
    digitalWrite(7, *data); 
}

//Assuming you have to Monitor application MonTest1-10, MonTest11-20, ConTestPin13, ConTestPin7
//For control application, you also want to pass the function pointer to perform the action
//create global scoped App array
App apps[] = {
    App("MonTest1-10", MONITOR_APP),
    App("MonTest11-20", MONITOR_APP),
    App("ConTestPin13", CONTROL_APP, lightUp13),
    App("ConTestPin7", CONTROL_APP, lightUp7)
};

//create global object confreak, Confreak will Open Serial if not opened already
Confreak g_freak(apps, sizeof(apps)/sizeof(apps[0]));

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(7, OUTPUT);
    Serial.begin(9600);
}

//dummy test function return a random number from 1 to 10
//In reality this can be the function that actually return
//some meanful data
int readOne()
{
    return random(1, 10);
}

int readTwo()
{
    return random(11, 20);
}

void loop()
{
    delay(2000);                                        //Recommended framerate: 2 seconds
    g_freak.readCommand();                              //Read command from C++ service and perform actions
    g_freak.updateAppData("MonTest1-10", readOne());    //update app data by the name
    g_freak.updateAppData("MonTest11-20", readTwo());
    g_freak.sendUpdate();                               //Sends the update over serial
}

//------ end of CONFREAK.ino ------
