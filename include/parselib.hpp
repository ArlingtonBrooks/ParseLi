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

/** @file
 * @brief Parser library header file
 * 
 * This file contains the Dict class definition as well as functions for 
 * loading configuration files.
 */
#ifndef PARSELIB_HPP_
#define PARSELIB_HPP_ 1

#include <iostream> //std::cout, std::cerr
#include <iomanip> //std::setw, std::left
#include <fstream> //std::ifstream
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map> //std::unordered_map

/** @namespace ParseLi 
 * @brief The namespace encompassing the ParseLi functions
 */
namespace ParseLi {

/** @class Dict
 * @brief A class contining information loaded from a configuration file
 * 
 * This class creates a set of unordered maps which translate a `std::string` key
 * 	into `double`, `int`, or `std::string` values read from an input
 *	configuration file.  
 * @todo It may be beneficial to add separate maps for value enforcement.
 * @{
*/
class Dict
{
	std::unordered_map<std::string,double> DoubleMap;       ///<Dictionary containing doubles
	std::unordered_map<std::string,int> IntMap;             ///<Dictionary containing ints
	std::unordered_map<std::string,std::string> StringMap;  ///<Dictionary containing strings

	public:
	/** @brief Thread access controller */
	mutable std::mutex DictMutex;
	
	/** @brief Default constructor */
	Dict() = default;
	~Dict() = default;
	
	/** @brief Copy constructor */
	Dict(const Dict &D) 
	{
		DoubleMap = D.DoubleMap;
		IntMap = D.IntMap;
		StringMap = D.StringMap;
	}
	
	/** @brief Assignment operator */
	Dict& operator=(const Dict &D)
	{
		DoubleMap = D.DoubleMap;
		IntMap = D.IntMap;
		StringMap = D.StringMap;
		return *this;
	}
	
	/** @brief Add a `double` to the dictionary map */
	bool add(std::string key, double val);
	/** @overload bool add(std::string key, int val); */
	bool add(std::string key, int val);
	/** @overload bool add(std::string key, std::string val); */
	bool add(std::string key, std::string val);

	/** @brief Get the double corresponding to `key` */
	double GetDouble(std::string key) const;
	/** @brief Get the int corresponding to `key` */
	int GetInt(std::string key) const;
	/** @brief Get the string corresponding to `key` */
	std::string GetString(std::string key) const;
	
	/** @brief Check if `key` exists in `double`s map */
	bool CheckDouble(std::string key) const;
	/** @brief Check if `key` exists in `int`s map*/
	bool CheckInt(std::string key) const;
	/** @brief Check if `key` exists in `std::string`s map */
	bool CheckString(std::string key) const;

	/** @brief Prints information about loaded dictionary to stdout */
	void Dump() const;
};
/** @} */

/**
 * @brief Skips whitespace in a std::string and increments a position indicator accordingly
 */
bool SkipStringWhitespace(std::string &LineData, int &siter);

/**
 * @brief Reads a word from a std::string and increments a position indicator accordingly (terminates on whitespace)
 */
std::string ReadValue(std::string LineData, int& siter);

/**
 * @brief Parses a std::string to determine what type of information is contained (string, int, or float)
 */
void GetValueType(bool* ValCheck, std::string VarVal);

/**
 * @brief Stores a value given by VarVal in the dictionary with key VarName
 */
bool StoreValue(Dict* D, std::string VarName, std::string VarVal, int ln, const char* BUFFER, bool DEBUG = false);

/**
 * @brief Enforces the value of a string (with some limitations)
 */
bool ValueEnforcer(const char* filename, Dict* D, int &siter, std::string &VarVal, std::string &LineData);

/**
 * @brief Handles a set of input data
 */
bool ValueHandler(const char* filename, const char* BUFFER, int &ln, Dict* D, int &siter, std::string &VarName, std::string &VarVal, std::string &LineData, bool DEBUG = false);

/**
 * @brief Reads a configuration file into a Dict pointer
*/
bool ReadConfig(const char* filename, Dict* D, bool DEBUG = false);

/**
 * @brief Reads from any input stream (istream) into a Dict pointer
*/
bool ReadConfig(std::istream &f_in, Dict* D, bool DEBUG =false);
} //namespace ParseLi

#endif
