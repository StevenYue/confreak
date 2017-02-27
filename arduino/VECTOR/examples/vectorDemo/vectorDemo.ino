//Steeng: Sun Feb 26 16:01:28 EST 2017
#include <VECTOR_LIB.h>

void setup()
{
    Serial.begin(9600);
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    Serial.println("Vector of Integer:");
    for ( int i = 0; i < v.size(); ++i )
    {
        Serial.println(v[i]); 
    }
    Vector<String> vs;
    vs.push_back("Arduino");
    vs.push_back("Vector");
    vs.push_back("Demo");
    Serial.println("Vector of String:");
    for ( int i = 0; i < vs.size(); ++i )
    {
        Serial.println(vs[i]); 
    }
}

void loop()
{
}

//------ end of vectorDemo.ino ------
