//Steeng: Sun Feb 26 15:48:42 EST 2017
#include "CONFREAK_LIB.h"

//Assuming you have to Monitor application MonTest1-10, MonTest11-20,
//create global scoped App array
App apps[] = {
    App("MonTest1-10", MONITOR_APP),
    App("MonTest11-20", MONITOR_APP)
};

//create global object confreak, Confreak will Open Serial if not opened already
Confreak g_freak(apps, sizeof(apps)/sizeof(apps[0]));

void setup()
{
    pinMode(13, OUTPUT);
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
    delay(2000);                                        //framerate 2 seconds
    if ( Serial.available() )
    {
        String s = Serial.readString();
        if ( s.charAt(s.length()-1) == '1' )
        {
            digitalWrite(13, 1); 
        }
        else
        {
            digitalWrite(13, 0); 
        }
    }

    g_freak.updateAppData("MonTest1-10", readOne());    //update app data by the name
    g_freak.updateAppData("MonTest11-20", readTwo());
    g_freak.sendUpdate();                               //Sends the update over serial
}

//------ end of CONFREAK.ino ------
