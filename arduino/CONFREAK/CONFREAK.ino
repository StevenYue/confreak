//Steeng: Sun Feb 26 15:48:42 EST 2017
#include "CONFREAK_LIB.h"
/***Confreak_lib only works in and after arduino 1.6.6, due to the include lib from lib issue***/

//Assuming you have a control application called "ConAppTest" and 
//a monitor application called "MonAppTest", ideally only Monitor
//App should be initilized here, for control app to reduce the duty
//of parsing string in arduino, there should be an easier way for 
//arduino
App apps[] = {
    App("ConAppTest", CONTROL_APP), 
    App("MonAppTest", MONITOR_APP)
};

//create global object confreak
Confreak g_freak(apps, sizeof(apps)/sizeof(apps[0]));

void setup()
{
    pinMode(13, OUTPUT);
    Serial.begin(9600);
}

//dummy test function return a random number from 0 to 10
int readTemperature()
{
    return random(0, 10);
}

void loop()
{
    delay(2000);                                        //framerate 2 seconds
    App* app = g_freak.getAppByName("MonAppTest");      //get the app pointer by its name
    if ( app )                                          //Not a null pointer
    {
        app->data = readTemperature();               //update global confreak object
        Serial.println(g_freak.toString().c_str());   //send the global confreak object over serial
    }
    else
    {
        Serial.println("null ptr");
    }
}

//------ end of CONFREAK.ino ------
