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

#include "parselib.hpp" //Dict

namespace ParseLi {

/**
 * @param key Lookup value corresponding to `val`
 * @param val Value added with key `key`
 * @return `True` if successful, `False` otherwise
*/
bool Dict::add(std::string Key, double value)
{
	DictMutex.lock();
	DoubleMap.emplace(Key,value);
	DictMutex.unlock();
	return true;
}

//integer overload for Dict::add
bool Dict::add(std::string Key, int value)
{
	DictMutex.lock();
	IntMap.emplace(Key,value);
	DictMutex.unlock();
	return true;
}

//std::string overload for Dict::add
bool Dict::add(std::string Key, std::string value)
{
	DictMutex.lock();
	StringMap.emplace(Key,value);
	DictMutex.unlock();
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
	DictMutex.lock();
	try {ret = DoubleMap.at(key);}
	catch (const std::out_of_range& e){
		std::cerr << "Value \"" << key << "\" out of range of doubles map (maybe this isn't a double?)" << std::endl;
		DictMutex.unlock();
		throw;
	}
	DictMutex.unlock();
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
	DictMutex.lock();
	try {ret = IntMap.at(key);}
	catch (const std::out_of_range& e) {
		std::cerr << "Value \"" << key << "\" out of range of int map (maybe this isn't a int?)" << std::endl;
		DictMutex.unlock();
		throw;
	}
	DictMutex.unlock();
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
	DictMutex.lock();
	try {ret = StringMap.at(key);}
	catch (const std::out_of_range& e){
		std::cerr << "Value \"" << key << "\" out of range of string map (maybe this isn't a string?)" << std::endl;
		DictMutex.unlock();
		throw;
	}
	DictMutex.unlock();
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
	using std::cout, std::endl, std::setw;
	cout << "Dictionary Dump" << endl << endl << "+->Integer Database" << endl;
	//Dump Integers
	cout << "+--->Size: " << IntMap.size() << endl;
	cout << "+---># of Buckets: " << IntMap.bucket_count() << endl;
	cout << "+--->Load Factor: " << IntMap.load_factor() << endl;
	for (auto i : IntMap)// = IntMap.begin(); i != IntMap.end(); ++i)
		cout << setw(20) << std::left << i.first << ": " << i.second << endl;

	//Dump Floats
	cout << "+--->Size: " << DoubleMap.size() << endl;
	cout << "+---># of Buckets: " << DoubleMap.bucket_count() << endl;
	cout << "+--->Load Factor: " << DoubleMap.load_factor() << endl;
	for (auto i : DoubleMap)// = DoubleMap.begin(); i != DoubleMap.end(); ++i)
		cout << setw(20) << std::left << i.first << ": " << i.second << endl;

	//Dump Strings
	cout << "+--->Size: " << StringMap.size() << endl;
	cout << "+---># of Buckets: " << StringMap.bucket_count() << endl;
	cout << "+--->Load Factor: " << StringMap.load_factor() << endl;
	for (auto i : StringMap)// = StringMap.begin(); i != StringMap.end(); ++i)
		cout << setw(20) << std::left << i.first << ": " << i.second << endl;
}

/**
 * @param LineData      Line data being parsed
 * @param siter         Current string iterator position (incremented to skip whitespace)
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
 * @param LineData      Line data being parsed
 * @param siter         Current string iterator position (incremented to end of word)
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
 * @param ValCheck      Vector of 3 bools indicating if (0) contains numbers, (1) contains a decimal point, (2) contains other characters that are non-numeric
 * @param VarVal        String being tested
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
 * @param D             Dictionary object being manipulated
 * @param VarName       Variable name being added to dictionary
 * @param VarVal        Value being added (as std::string; this will be parsed internally)
 * @param ln            Line number (for error handling)
 * @param BUFFER        Character array containing line data (for error handling)
 * @param DEBUG         Error handling flag
 * @returns `true` on successful addition of a value to the dictionary.
 * @returns `false` if a value was not able to be added to the dictionary.
 */
bool StoreValue(Dict* D, std::string VarName, std::string VarVal, int ln, const char* BUFFER, bool DEBUG /*=false*/)
{
	using std::cerr, std::endl;
	bool ValCheck[3] {0,0,0}; //int, double, string;
	GetValueType(ValCheck,VarVal);
	if (ValCheck[2]) { //String add
		D->add(VarName,VarVal);
		if (DEBUG) cerr << "String added: " << VarName << ": " << VarVal << endl;
	} else if (ValCheck[1]) { //(double) float add
		try {D->add(VarName,std::stod(VarVal.c_str()));}
		catch (const std::invalid_argument &e) {
			cerr << "Failed to parse double " << VarVal << " on line " << ln << ":" << endl;
			cerr << BUFFER << endl;
			return false;
		} catch (const std::out_of_range &e) {
			cerr << "Double " << VarVal << " on line " << ln << " is out of range." << endl;
			return false;
		}
		if (DEBUG) cerr << "Float added: " << VarName << ": " << VarVal << endl;
	} else if (ValCheck[0]) { //int add
		try {D->add(VarName,std::stoi(VarVal.c_str()));}
		catch (const std::invalid_argument &e) {
			cerr << "Failed to parse int " << VarVal << " on line " << ln << ":" << endl;
			cerr << BUFFER << endl;
			return false;
		} catch (const std::out_of_range &e) {
			cerr << "Int " << VarVal << " on line " << ln << " is out of range." << endl;
			return false;
		}
		if (DEBUG) cerr << "Int added: " << VarName << ": " << VarVal << endl;
	}
	return true;
}

/**
 * @param filename      Name of file being loaded
 * @param D             Dictionary where information is loaded to
 * @param siter         Current string iterator position (incremented to end of word)
 * @param VarVal        Name of enforced variable being tested
 * @param LineData      Current line being parsed
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
			std::cerr << "Error enforcing " << Enforcer << " in filename " << filename; 
			std::cerr << ": value mismatch (" << EnforceVal << " vs " << D->GetString(Enforcer) << ")" << std::endl;
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
 * @param filename      Name of file being loaded
 * @param BUFFER        Raw line data buffer
 * @param ln            Line number
 * @param D             Dictionary where information is loaded to
 * @param siter         Current string iterator position (incremented to end of word)
 * @param VarName       Name of variable being handled
 * @param VarVal        Value of variable being handled
 * @param LineData      Current line being parsed
 * @param DEBUG         Whether to print debug info
 * @returns `true` if line data was handled successfully
 * @returns `false` if something went wrong (outputs to std::cerr)
 */
bool ValueHandler(const char* filename, const char* BUFFER, int &ln, Dict* D, int &siter, std::string &VarName, std::string &VarVal, std::string &LineData, bool DEBUG /*=false*/) 
{
	using std::cerr, std::endl;
	//Include handler (THIS DOES NOT DETECT RECURSION)
	//Recursion detection could be implemented by keeping a running tab of open files
	if (VarName.compare("include") == 0 || VarName.compare("INCLUDE") == 0) {
		if (VarVal.compare(filename) == 0) {
			cerr << "Error in file include: Filename " << VarVal << " cannot include itself (line " << ln << ")" << endl;
			return true;
		}
		ReadConfig(VarVal.c_str(),D,DEBUG);
		return true;
	} 
	//Warning handler
	if (VarName.compare("WARNING") == 0 || VarName.compare("warning") == 0) {
		cerr << "\x1b[1mWARNING\x1b[0m:";
		cerr << LineData.substr(LineData.find(' ')) << endl;
		return true;
	}
	
	//Enforcement handler (only handles strings)
	if (VarName.compare("enforce") == 0 || VarName.compare("ENFORCE") == 0) {
		bool EnforceSuccess = ValueEnforcer(filename, D, siter, VarVal, LineData);
		if (EnforceSuccess)
			return true;
		else
			return false;
	}
	//Store in dictionary
	if (!StoreValue(D,VarName,VarVal,ln,BUFFER,DEBUG))
		return false;
	
	return true;
}

/**
 * @param filename      Name of the configuration file to read
 * @param D             Dictionary where information is loaded to
 * @param DEBUG         Whether to print debugging information
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
	using std::cerr, std::endl;
	//Check file exists (old C-way)
	FILE *f{fopen(filename,"r")};
	if (f == NULL) return false;
	else fclose(f);

	char BUFFER[512];

	//Open file for reading
	std::ifstream f_in(filename,std::ifstream::in);
	if (DEBUG) cerr << "Opened " << filename << " for input" << endl;
	
	return ReadConfig(f_in,D,DEBUG);
};

/**
 * @param f_in          Generic stream file input
 * @param D             Dictionary where information is loaded to
 * @param DEBUG         Whether to print debugging information
 * @return `True` on successful read of config
 * @return `False` if an error occurs, with accompanying output to stdout
*/
bool ReadConfig(std::istream &f_in, Dict* D, bool DEBUG /*=false*/)
{
	using std::cerr, std::endl;
	f_in.seekg(0,f_in.beg);
	const char* filename = "(streamed input)";
	char BUFFER[512];

	int ln{0};
	if (DEBUG) cerr << "Opened " << filename << " for input" << endl;

	while (f_in) {
		std::string VarName;
		std::string VarVal;
		int siter {0}; //string iter
		ln += 1;
		//Read line from buffer;
		f_in.getline(BUFFER,512);

		//Return if file failed to read;
		if (f_in.fail() && !f_in.eof()) {
			cerr << "An error occurred while reading " << filename << ".  Failed to load." << endl;
			cerr << "Characters in buffer: " << endl << BUFFER << endl;
			return false;
		}
		if (f_in.eof())
			break;

		if (DEBUG) cerr << "(" << ln << "): " << BUFFER << endl;

		std::string LineData = std::string(BUFFER);
		if (!SkipStringWhitespace(LineData,siter))
			continue;

		//Read variable name
		VarName = ReadValue(LineData,siter);
		if (VarName.compare("BREAK") == 0) { //Stop reading on break signal
			if (DEBUG) cerr << "Encountered \"BREAK\" signal.  Terminating input." << endl;
			break;
		}
		if (VarName.length() < 1)
			continue;
		
		//Read variable value
		if (!SkipStringWhitespace(LineData,siter)) {
			cerr << "Reached end of line " << ln << "while parsing variable: " << LineData << endl;
			return false;
		}
		VarVal = ReadValue(LineData,siter);
		
		if (!ValueHandler(filename, BUFFER, ln, D, siter, VarName, VarVal, LineData, DEBUG))
			return false;
	}
	if (DEBUG) cerr << "Completed parsing (streamed input)" << endl;
	return true;
};

} //namespace ParseLi

#endif
