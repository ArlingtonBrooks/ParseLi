/*
    Copyright (C) 2019-2022  ArlingtonBrooks

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
bool ReadConfig(const char* filename, Dict* D, bool DEBUG);
}

#endif
