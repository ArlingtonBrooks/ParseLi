#ifndef PARSELIB_HPP_
#define PARSELIB_HPP_ 1

#include <string>
#include <unordered_map> //std::unordered_map

extern "C" {

/*
Dictionary class
    Makes loading values from a config file easier
    (Access by "key" names which return associated values)
*/
class Dict
{
    std::unordered_map<std::string,double> DoubleMap;
    std::unordered_map<std::string,int> IntMap; 
    std::unordered_map<std::string,std::string> StringMap;

    public:
    bool add(std::string, double);
    bool add(std::string, int);
    bool add(std::string, std::string);

    double GetDouble(std::string key);
    int GetInt(std::string key);
    std::string GetString(std::string key);

    void Dump();
};

//extern "C" bool ReadConfig(const char* filename, Dict* D);
bool ReadConfig(const char* filename, Dict* D, bool DEBUG=false);
}

#endif
