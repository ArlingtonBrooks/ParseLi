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

/*
	This library was initially developed in 2016 for a personal project, 
	however has been updated recently to suit new demands.
	Initial release under GPL was in 2019.
	Initial git commit to GitHub was in 2022.
*/

#ifndef PARSELIB_CPP_
#define PARSELIB_CPP_ 1

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "parselib.hpp" //Dict

namespace ParseLi {

/**
 * @param key Lookup value corresponding to `val`
 * @param val Value added with key `key`
 * @return `True` if successful, `False` otherwise
*/
bool Dict::add(std::string Key, double value)
{
	DoubleMap.emplace(Key,value);
	return true;
}

//integer overload for Dict::add
bool Dict::add(std::string Key, int value)
{
	IntMap.emplace(Key,value);
	return true;
}

//std::string overload for Dict::add
bool Dict::add(std::string Key, std::string value)
{
	StringMap.emplace(Key,value);
	return true;
}

/**
 * @param key Value to look up
 * @return Value corresponding to `key`
 * @throw std::out_of_range if value cannot be found
*/
double Dict::GetDouble(std::string key) const
{
	double ret;
	try {ret = DoubleMap.at(key);}
	catch (const std::out_of_range& e){
		fprintf(stderr,"Value \"%s\" out of range of doubles map (maybe this isn't a double?)\n",key.c_str());
		throw;
	}
	return ret;
}

/**
 * @param key Value to look up
 * @return Value corresponding to `key`
 * @throw std::out_of_range if value cannot be found
*/
int Dict::GetInt(std::string key) const
{
	int ret;
	try {ret = IntMap.at(key);}
	catch (const std::out_of_range& e) {
		fprintf(stderr,"Value \"%s\" out of range of int map (maybe this isn't an int?)\n",key.c_str());
		throw;
	}
	return ret;
}

/**
 * @param key Value to look up
 * @return Value corresponding to `key`
 * @throw std::out_of_range if value cannot be found
*/
std::string Dict::GetString(std::string key) const
{
	std::string ret;
	try {ret = StringMap.at(key);}
	catch (const std::out_of_range& e){
		fprintf(stderr,"Value \"%s\" out of range of strings map (maybe this isn't a string?)\n",key.c_str());
		throw;
	}
	return ret;
}

/**
 * @param key Value to look up
 * @return `True` if `key` exists, `False` otherwise
*/
bool Dict::CheckDouble(std::string key) const
{
	try {
		DoubleMap.at(key);
		return true;
	} catch (const std::out_of_range& e){
		return false;
	}
}

/**
 * @param key Value to look up
 * @return `True` if `key` exists, `False` otherwise
*/
bool Dict::CheckInt(std::string key) const
{
	try {
		IntMap.at(key);
		return true;
	} catch (const std::out_of_range& e){
		return false;
	}
}

/**
 * @param key Value to look up
 * @return `True` if `key` exists, `False` otherwise
*/
bool Dict::CheckString(std::string key) const
{
	try {
		StringMap.at(key);
		return true;
	} catch (const std::out_of_range& e){
		return false;
	}
}

//Printout all values from the dictionary
void Dict::Dump() const
{
	printf("Dictionary Dump\n\n+->Integer Database\n");
	//Dump Integers
	printf("+--->Size: %lu\n+---># of Buckets: %lu\n+--->Load Factor: %f\n+--->Entries:\n",IntMap.size(),IntMap.bucket_count(),IntMap.load_factor());
	for (auto i : IntMap)// = IntMap.begin(); i != IntMap.end(); ++i)
		printf("		  %s: %d\n",i.first.c_str(),i.second);

	printf("\n\n+->Float Database\n");
	//Dump Integers
	printf("+--->Size: %lu\n+---># of Buckets: %lu\n+--->Load Factor: %f\n+--->Entries:\n",DoubleMap.size(),DoubleMap.bucket_count(),DoubleMap.load_factor());
	for (auto i : DoubleMap) // = DoubleMap.begin(); i != DoubleMap.end(); ++i)
		printf("		  %s: %lf\n",i.first.c_str(),i.second);

	printf("\n\n+->String Database\n");
	//Dump Integers
	printf("+--->Size: %lu\n+---># of Buckets: %lu\n+--->Load Factor: %f\n+--->Entries:\n",StringMap.size(),StringMap.bucket_count(),StringMap.load_factor());
	for (auto i : StringMap)// = StringMap.begin(); i != StringMap.end(); ++i)
		printf("		  %s: %s\n",i.first.c_str(),i.second.c_str());
}

/**
 * @param LineData	Line data being parsed
 * @param siter		Current string iterator position (incremented to skip whitespace)
 * @returns `true` if whitespace is skipped and input is ready to be parsed
 * @returns `false` if a comment indicator or null character is encountered
 */
bool SkipStringWhitespace(std::string &LineData, int &siter)
{
	while ((LineData[siter] == ' ' || LineData[siter] == '\t') && LineData[siter] != '\0' && LineData[siter] != '#')
		siter++;
	if (LineData[siter] == '\0' || LineData[siter] == '#')
		return false;
	return true;
}

/**
 * @param LineData	Line data being parsed
 * @param siter		Current string iterator position (incremented to end of word)
 * @returns std::string containing the value read (excluding whitespace characters)
 */
std::string ReadValue(std::string LineData, int& siter)
{
	std::string Value;
	while (LineData[siter] != '\n' && LineData[siter] != '\t' && LineData[siter] != '#' && LineData[siter] != '\0' && LineData[siter] != ' ') {
		Value += LineData[siter];
		siter++;
	}
	return Value;
}

/**
 * @param ValCheck	Vector of 3 bools indicating if (0) contains numbers, (1) contains a decimal point, (2) contains other characters that are non-numeric
 * @param VarVal	String being tested
 */
void GetValueType(bool* ValCheck, std::string VarVal)
{
	bool ExpForm = false; //Whether string is already categorized as being in exponential form
	ValCheck[0] = false; //Contains numbers
	ValCheck[1] = false; //Contains decimal point
	ValCheck[2] = false; //Contains string data
	for (unsigned int i{0}; i < VarVal.length(); i++) {
		if (VarVal[i] != ' ') {
			if (!ValCheck[0] && ((VarVal[i] >= '0' && VarVal[i] <= '9') || VarVal[i] == '-'))
				ValCheck[0] = true; //Contains integers;
			else if (!ValCheck[1] && VarVal[i] == '.')
				ValCheck[1] = true; //Contains decimal
			else if ((!ValCheck[2] && !((VarVal[i] >= '0' && VarVal[i] <= '9') || VarVal[i] == '-' || VarVal[i] == '+' || VarVal[i] == '.')) || ((VarVal[i] == '.' && ValCheck[1]))) {
				if (VarVal[i] == 'e' || VarVal[i] == 'E') { //Contains exponent
					if (ExpForm) {
						ValCheck[2] = true;
						break; //Nothing can override a string;  Break if this happens.
					} else {
						ValCheck[1] = true;
						ExpForm = true;
					}
				} else {
					ValCheck[2] = true;
					break; //Nothing can override a string;  Break if this happens.
				}
			}
		}
	}
}

/**
 * @param D		Dictionary object being manipulated
 * @param VarName	Variable name being added to dictionary
 * @param VarVal	Value being added (as std::string; this will be parsed internally)
 * @param ln		Line number (for error handling)
 * @param BUFFER	Character array containing line data (for error handling)
 * @param DEBUG		Error handling flag
 * @returns `true` on successful addition of a value to the dictionary.
 * @returns `false` if a value was not able to be added to the dictionary.
 */
bool StoreValue(Dict* D, std::string VarName, std::string VarVal, int ln, const char* BUFFER, bool DEBUG /*=false*/)
{
	bool ValCheck[3] {0,0,0}; //int, double, string;
	GetValueType(ValCheck,VarVal);
	if (ValCheck[2]) { //String add
		D->add(VarName,VarVal);
		if (DEBUG)
			fprintf(stderr,"String added: %s:\t%s\n",VarName.c_str(),VarVal.c_str());
	} else if (ValCheck[1]) { //(double) float add
		try {D->add(VarName,std::stod(VarVal.c_str()));}
		catch (const std::invalid_argument &e) {
			fprintf(stderr,"Failed to parse double %s on line %d:\n",VarVal.c_str(),ln);
			fprintf(stderr,"%s",BUFFER);
			return false;
		} catch (const std::out_of_range &e) {
			fprintf(stderr,"Double %s on line %d is out of range.\n",VarVal.c_str(),ln);
			return false;
		}
		if (DEBUG)
			fprintf(stderr,"Float added: %s:\t%s\n",VarName.c_str(),VarVal.c_str());
	} else if (ValCheck[0]) { //int add
		try {D->add(VarName,std::stoi(VarVal.c_str()));}
		catch (const std::invalid_argument &e) {
			fprintf(stderr,"Failed to parse int %s on line %d:\n",VarVal.c_str(),ln);
			fprintf(stderr,"%s",BUFFER);
			return false;
		} catch (const std::out_of_range &e) {
			fprintf(stderr,"Int %s on line %d is out of range.\n",VarVal.c_str(),ln);
			return false;
		}
		if (DEBUG)
			fprintf(stderr,"Int added: %s:\t%s\n",VarName.c_str(),VarVal.c_str());
	}
	return true;
}

/**
 * @param filename	Name of file being loaded
 * @param D		Dictionary where information is loaded to
 * @param siter		Current string iterator position (incremented to end of word)
 * @param VarVal	Name of enforced variable being tested
 * @param LineData	Current line being parsed
 * @returns `true` if enforced value is satisfied (or has been added to dictionary)
 * @returns `false` if enforcement is violated by a prior definition
 */
bool ValueEnforcer(const char* filename, Dict* D, int &siter, std::string &VarVal, std::string &LineData) {
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
			return false;
		}
	} else {
		while (LineData[siter] != '\n' && LineData[siter] != '\t' && LineData[siter] != '#' && LineData[siter] != '\0' && LineData[siter] != ' ') {
			EnforceVal += LineData[siter];
			siter++;
		}
		D->add(Enforcer,EnforceVal);
	}
	return true;
}

/**
 * @param filename 	Name of the configuration file to read
 * @param D 		Dictionary where information is loaded to
 * @param DEBUG 	Whether to print debugging information
 * @return `True` on successful read of config file
 * @return `False` if an error occurs, with accompanying output to stdout
 *
 * @note The following code block represents an example input file
 * 	which will produce a collection of `int`s, `double`s, and
 *	`std::string`s.
 * @note Enforcement handling only checks that the variable has not been 
 * 	differently defined prior to the `ENFORCE` call.  If the variable
 *	is defined afterwards, it is overwritten.  
 *	This could be mitigated by adding a separate dictionary for all
 *	enforced values.
 * @note Enforcement only applies to 'std::string' types.
 * @warning The `include` keyword does not have recursion detection.
 * @note Example inputs:
 * @code{.unparsed}
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
		MAXITER 100000; #This is stored as an integer
 * @endcode
*/
bool ReadConfig(const char* filename, Dict* D, bool DEBUG /*=false*/)
{
	//Check file exists (old C-way)
	FILE *f{fopen(filename,"r")};
	if (f == NULL) return false;
	else fclose(f);

	char BUFFER[512];

	//Open file for reading
	std::ifstream f_in(filename,std::ifstream::in);

	int ln{0};
	if (DEBUG)
		fprintf(stderr,"Opened %s for input\n",filename);

	while (f_in.good()) {
		std::string VarName;
		std::string VarVal;
		int siter {0}; //string iter
		ln += 1;
		//Read line from buffer;
		f_in.getline(BUFFER,512);

		//Return if file failed to read;
		if (f_in.fail() && !f_in.eof()) {
			fprintf(stderr,"An error occurred while reading file %s.  Failed to load.\n",filename);
			fprintf(stderr,"Characters in buffer: \n\t%s\n",BUFFER);
			return false;
		}
		if (f_in.eof())
			break;

		if (DEBUG)
			fprintf(stderr,"(%d): %s\n",ln,BUFFER);

		std::string LineData = std::string(BUFFER);
		if (!SkipStringWhitespace(LineData,siter))
			continue;

		//Read variable name
		VarName = ReadValue(LineData,siter);
		if (VarName.length() < 1)
			continue;
		
		//Read variable value
		if (!SkipStringWhitespace(LineData,siter)) {
			fprintf(stderr,"Reached end of line %d while parsing variable: %s\n",ln,LineData.c_str());
			return false;
		}
		VarVal = ReadValue(LineData,siter);
		
		//Include handler (THIS DOES NOT DETECT RECURSION)
		//Recursion detection could be implemented by keeping a running tab of open files
		if (VarName.compare("include") == 0 || VarName.compare("INCLUDE") == 0) {
			if (VarVal.compare(filename) == 0) {
				fprintf(stderr,"Error in file include: Filename %s cannot include itself (line %d)\n",VarVal.c_str(),ln);
				continue;
			}
			ReadConfig(VarVal.c_str(),D,DEBUG);
			continue;
		}
		
		//Enforcement handler (only handles strings)
		if (VarName.compare("enforce") == 0 || VarName.compare("ENFORCE") == 0) {
			bool EnforceSuccess = ValueEnforcer(filename, D, siter, VarVal, LineData);
			if (EnforceSuccess)
				continue;
			else
				return false;
		}
		//Store in dictionary
		if (!StoreValue(D,VarName,VarVal,ln,BUFFER,DEBUG))
			return false;
	}
	if (DEBUG)
		fprintf(stderr,"Closed %s for input\n",filename);
	f_in.close();
	return true;
};

} //namespace ParseLi

#endif
