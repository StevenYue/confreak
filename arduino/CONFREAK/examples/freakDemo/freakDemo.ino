//Steeng: Sun Feb 26 15:48:42 EST 2017
#include "CONFREAK_LIB.h"

//Assuming you have a control application called "ConAppTest" and 
//a monitor application called "MonAppTest"
App apps[] = {
    App("ConAppTest", CONTROL_APP), 
    App("MonAppTest", MONITOR_APP)
};
Confreak g_freak(apps, sizeof(apps)/sizeof(apps[0]));

void setup()
{
    pinMode(13, OUTPUT);
    Serial.begin(9600);
}

//dummy test function return a random number from 0 to 10


void loop()
{
    delay(2000);
    Serial.println("11111");
    Serial.println(g_freak.toJsonString());
    Serial.println("22222");
}

//------ end of freakDemo.ino ------
