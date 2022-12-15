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

#ifndef PARSELIB_CPP_
#define PARSELIB_CPP_ 1

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "parselib.hpp" //Dict

/*
Dictionary class
    Makes loading values from a config file easier
    (Access by "key" names which return associated values)
*/

//Add 'double' to dictionary
bool Dict::add(std::string Key, double value)
{
    DoubleMap.emplace(Key,value);
    return true;
}

//Add 'int' to dictionary
bool Dict::add(std::string Key, int value)
{
    IntMap.emplace(Key,value);
    return true;
}

//Add 'string' to dictionary
bool Dict::add(std::string Key, std::string value)
{
    StringMap.emplace(Key,value);
    return true;
}

//Read 'double' from dictionary
//Throw std::out_of_range exception on failure
double Dict::GetDouble(std::string key)
{
    double ret;
    try {ret = DoubleMap.at(key);}
    catch (const std::out_of_range& e){
        fprintf(stderr,"Value \"%s\" out of range of doubles map (maybe this isn't a double?)\n",key.c_str());
        throw;
    }
    return ret;
}

//Read 'int' from dictionary
//Throw std::out_of_range exception on failure
int Dict::GetInt(std::string key)
{
    int ret;
    try {ret = IntMap.at(key);}
    catch (const std::out_of_range& e) {
        fprintf(stderr,"Value \"%s\" out of range of int map (maybe this isn't an int?)\n",key.c_str());
        throw;
    }
    return ret;
}

//Read 'string' from dictionary
//Throw std::out_of_range exception on failure
std::string Dict::GetString(std::string key)
{
    std::string ret;
    try {ret = StringMap.at(key);}
    catch (const std::out_of_range& e){
        fprintf(stderr,"Value \"%s\" out of range of strings map (maybe this isn't a string?)\n",key.c_str());
        throw;
    }
    return ret;
}

//Check if a double exists; return true if it does
bool Dict::CheckDouble(std::string key)
{
    try {
        DoubleMap.at(key);
        return true;
    } catch (const std::out_of_range& e){
        return false;
    }
}

//Check if an int exists; return true if it does
bool Dict::CheckInt(std::string key)
{
    try {
        IntMap.at(key);
        return true;
    } catch (const std::out_of_range& e){
        return false;
    }
}

//Check if a string exists; return true if it does
bool Dict::CheckString(std::string key)
{
    try {
        StringMap.at(key);
        return true;
    } catch (const std::out_of_range& e){
        return false;
    }
}

//Printout all values from the dictionary
void Dict::Dump()
{
    printf("Dictionary Dump\n\n+->Integer Database\n");
    //Dump Integers
    printf("+--->Size: %lu\n+---># of Buckets: %lu\n+--->Load Factor: %f\n+--->Entries:\n",IntMap.size(),IntMap.bucket_count(),IntMap.load_factor());
    for (auto i : IntMap)// = IntMap.begin(); i != IntMap.end(); ++i)
        printf("          %s: %d\n",i.first.c_str(),i.second);

    printf("\n\n+->Float Database\n");
    //Dump Integers
    printf("+--->Size: %lu\n+---># of Buckets: %lu\n+--->Load Factor: %f\n+--->Entries:\n",DoubleMap.size(),DoubleMap.bucket_count(),DoubleMap.load_factor());
    for (auto i : DoubleMap) // = DoubleMap.begin(); i != DoubleMap.end(); ++i)
        printf("          %s: %lf\n",i.first.c_str(),i.second);

    printf("\n\n+->String Database\n");
    //Dump Integers
    printf("+--->Size: %lu\n+---># of Buckets: %lu\n+--->Load Factor: %f\n+--->Entries:\n",StringMap.size(),StringMap.bucket_count(),StringMap.load_factor());
    for (auto i : StringMap)// = StringMap.begin(); i != StringMap.end(); ++i)
        printf("          %s: %s\n",i.first.c_str(),i.second.c_str());
}

/*
ReadConfig
    Takes:
        Configuration filename (const char*)
        Global Config Dictionary
    Returns:
        1 upon successful reading of config file
        Builds a dictionary at Dict* D

    Format:
        #Any line starting with a '#' is considered a comment line.
        #First: define solver type:
        SOLVER REIMANN

        #Second: define initial conditions
        LPRESS 1025.232
        L_a 200. #'.' indicates this is float;

        RPRESS 201.5
        R_a 250. #'.' indicates this is float;
        
        #Define tolerance and max # iterations:
        TOL 0.000001;
        MAXITER 100000;
*/
bool ReadConfig(const char* filename, Dict* D, bool DEBUG = false)
{
    FILE *f{fopen(filename,"r")};
    if (f == NULL) return 0;
    else fclose(f);

    char BUFFER[512];

    std::ifstream f_in(filename,std::ifstream::in);

    int ln{0};
    if (DEBUG)
        fprintf(stderr,"Opened %s for input\n",filename);

    while (f_in.good()) {
        ln += 1;
        //Read line from buffer;
        f_in.getline(BUFFER,512);
        int iter {0};

        //Return if file failed to read;
        if (f_in.fail() && !f_in.eof()) {
            fprintf(stderr,"An error occurred while reading file %s.  Failed to load.\n",filename);
            fprintf(stderr,"Characters in buffer: \n\t%s\n",BUFFER);
            return 0;
        }
        if (f_in.eof())
            break;

        if (DEBUG)
            fprintf(stderr,"(%d): %s\n",ln,BUFFER);

        //Skip whitespace and do not read past termination or comment character;
        while ((BUFFER[iter] == ' ' || BUFFER[iter] == '\t') && BUFFER[iter] != '\0' && BUFFER[iter] != '#')
            iter++;

        //Read next line if this line has no content;
        if (BUFFER[iter] == '\0' || BUFFER[iter] == '#')
            continue;

        std::string LineData = std::string(BUFFER);
        //Skip whitespace;
        int siter{0};
        while (LineData[siter] == ' ' || LineData[siter] == '\t')
            siter++;
        //Read variable name
        std::string VarName;
        while (LineData[siter] != ' ' && LineData[siter] != '\t') {
            VarName += LineData[siter];
            siter++;
            if (LineData[siter] == '\0') {
                fprintf(stderr,"Reached end of line while parsing config file (line %d)\n",ln);
                f_in.close();
                return false;
            }
        }
        //Read variable value
        while (LineData[siter] == ' ' || LineData[siter] == '\t')
            siter++;
        std::string VarVal;
        while (LineData[siter] != '\n' && LineData[siter] != '\t' && LineData[siter] != '#' && LineData[siter] != '\0' && LineData[siter] != ' ') {
            VarVal += LineData[siter];
            siter++;
        }
        //Include handler
        if (VarName.compare("include") == 0) {
            if (VarVal.compare(filename) == 0) {
                fprintf(stderr,"Error in file include: Filename %s cannot include itself (line %d)\n",VarVal.c_str(),ln);
                continue;
            }
            ReadConfig(VarVal.c_str(),D,DEBUG);
            continue;
        }
        //Enforcement handler
        if (VarName.compare("enforce") == 0) {
            siter++;
            std::string Enforcer = VarVal;
            std::string EnforceVal;
            if (D->CheckString(Enforcer)) { //If enforcing a value, check if it is identical
                while (LineData[siter] != '\n' && LineData[siter] != '\t' && LineData[siter] != '#' && LineData[siter] != '\0' && LineData[siter] != ' ') {
                    EnforceVal += LineData[siter];
                    siter++;
                }
                if (EnforceVal.compare(D->GetString(Enforcer)) != 0) {
                    fprintf(stderr,"Error enforcing %s in filename %s: value mismatch (%s vs %s)\n",Enforcer.c_str(),filename,EnforceVal.c_str(),D->GetString(Enforcer).c_str());
                    throw 1;
                }
                continue;
            } else {
                while (LineData[siter] != '\n' && LineData[siter] != '\t' && LineData[siter] != '#' && LineData[siter] != '\0' && LineData[siter] != ' ') {
                    EnforceVal += LineData[siter];
                    siter++;
                }
                D->add(Enforcer,EnforceVal);
                continue;
            }
        }
        //Try to figure out what kind of variable this is (all #s = int, #s with a decimal = double, any characters at all = string)
        //  In the future, this may parse for units also.
        bool ValCheck[3] {0,0,0}; //int, double, string;
        bool ExpForm = false;
        for (unsigned int i{0}; i < VarVal.length(); i++) {
            if (VarVal[i] != ' ') {
                if (!ValCheck[0] && ((VarVal[i] >= '0' && VarVal[i] <= '9') || VarVal[i] == '-'))
                    ValCheck[0] = true; //Contains integers;
                else if (!ValCheck[1] && VarVal[i] == '.')
                    ValCheck[1] = true; //Contains decimal
                else if ((!ValCheck[2] && !((VarVal[i] >= '0' && VarVal[i] <= '9') || VarVal[i] == '-' || VarVal[i] == '+' || VarVal[i] == '.')) || ((VarVal[i] == '.' && ValCheck[1]))) {
                    if (VarVal[i] == 'e' || VarVal[i] == 'E') {
                        if (ExpForm) {
                            ValCheck[2] = true;
                            break; //Nothing can override a string;  Break if this happens.
                        } else {
                            ValCheck[1] = true;
                            ExpForm = true;
                        }
                    } else {
                        ValCheck[2] = true;
                        break;
                    }
                }
            }
        }

        //Store in dictionary
        if (ValCheck[2]) {
            D->add(VarName,VarVal);
            if (DEBUG)
                fprintf(stderr,"String added: %s:\t%s\n",VarName.c_str(),VarVal.c_str());
        } else if (ValCheck[1]) {
            try {D->add(VarName,std::stod(VarVal.c_str()));}
            catch (const std::invalid_argument &e) {
                fprintf(stderr,"Failed to parse double %s on line %d:\n",VarVal.c_str(),iter);
                fprintf(stderr,"%s",BUFFER);
                return false;
            } catch (const std::out_of_range &e) {
                fprintf(stderr,"Double %s on line %d is out of range.\n",VarVal.c_str(),iter);
                return false;
            }
            if (DEBUG)
                fprintf(stderr,"Float added: %s:\t%s\n",VarName.c_str(),VarVal.c_str());
        } else if (ValCheck[0]) {
            try {D->add(VarName,std::stoi(VarVal.c_str()));}
            catch (const std::invalid_argument &e) {
                fprintf(stderr,"Failed to parse int %s on line %d:\n",VarVal.c_str(),iter);
                fprintf(stderr,"%s",BUFFER);
                return false;
            } catch (const std::out_of_range &e) {
                fprintf(stderr,"Int %s on line %d is out of range.\n",VarVal.c_str(),iter);
                return false;
            }
            if (DEBUG)
                fprintf(stderr,"Int added: %s:\t%s\n",VarName.c_str(),VarVal.c_str());
        }
    }
    if (DEBUG)
        fprintf(stderr,"Closed %s for input\n",filename);
    f_in.close();
    return true;
};

#endif
