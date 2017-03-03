//Steeng created at Mon Feb 27 22:12:58 EST 2017
#ifndef INCLUDED_CONFREAK_ARDUINOTRANSLATOR
#define INCLUDED_CONFREAK_ARDUINOTRANSLATOR

#include <confreak_schema.h>
#include <string>

//The purpose of arduinoTranslator is to convert the json format info into
//some symbol delimited strings, or vice versa. 
//the ardunio format data will look like 
//  NAME|1|12.0,MINGZI|0|1
namespace confreak { 
namespace arduinoTranslator{

std::string toArduino(Application& app);

std::string toArduino(ConfreakApps& apps);

ConfreakRt toApplication(const std::string& arduinoStr, Application& app);

ConfreakRt toConfreakApps(const std::string& arduinoStr, ConfreakApps& apps);

}   //end of namespace arduinoTranslator
}   //end of namespace confreak
#endif
