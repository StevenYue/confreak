#ifndef INCLUDED_CONFIG_LOADER
#define INCLUDED_CONFIG_LOADER
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


class Value {
private:
    string d_val;
public:
    Value():d_val(""){}
    Value(const string& val):d_val(val){}
    int asInt() {std::stoi(d_val);} //stoi CAN THROW!!!
    const string& asString()   {return d_val;}
};

class Config {
private:
    unordered_map<string, unordered_map<string, Value> > d_data;
    static Config *d_config;
    Config(char* fileName)
    {
        string line;
        ifstream f(fileName);
        if (!f.is_open())
        {
            throw runtime_error("Error opening config file");
        }
        string sectionName;
        while (getline(f, line)) 
        {
            processLine(line, sectionName);
        }
        if ( f.bad() )
        {
            throw runtime_error("Error reading config file");
        }
        f.close();
    }

    void processLine(string& line, string& sectionName)
    {
        size_t foundEqual = line.find("=");
        size_t foundBrace = line.find("[");
        if ( foundBrace != string::npos )
        {
            unordered_map<string, Value> m;
            sectionName = line.substr(1, line.length() - 2);
            d_data.insert({sectionName, m});
        }
        else if ( foundEqual != string::npos )
        {
            auto iter = d_data.find(sectionName);
            if ( iter == d_data.end() )
            {
                throw runtime_error("Error parsing config file");
            }
            iter->second.insert({line.substr(0, foundEqual), Value(line.substr(foundEqual+1))});
        }
        else
        {}
    }

public:
    static Config* getInstance(char* fileName)
    {
        if ( !d_config )
        {
            d_config = new Config(fileName);
        }
        return d_config;
    }

    unordered_map<string, Value>& operator[](const string& blockName)
    {
        return d_data[blockName];
    }
};

#endif
