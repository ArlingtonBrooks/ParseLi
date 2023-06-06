/*
	A configuration file parsing library
	Copyright (C) 2023  Arlington Brooks

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
	USA
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

class Dict;
/**
 * @brief Reads a configuration file into a Dict pointer
*/
bool ReadConfig(const char* filename, Dict* D, bool DEBUG = false);

/**
 * @brief Reads from any input stream (istream) into a Dict pointer
*/
bool ReadConfig(std::istream &f_in, Dict* D, bool DEBUG =false);

/**
 * @brief Compares two std::strings ignoring case
 */
bool StringsEqualIgnoreCase(std::string const &S1, std::string const &S2);

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
	std::unordered_map<std::string,std::string> StringMap;  ///<Dictionary containing strings
	std::unordered_map<std::string,double> DoubleMap;       ///<Dictionary containing doubles
	std::unordered_map<std::string,int> IntMap;             ///<Dictionary containing ints

	/** @brief Thread access controller */
	mutable std::mutex DictMutex;
	public:
	std::string Filename;
	
	/** @brief Default constructor */
	Dict() = default;
	Dict(std::string const &Filename_) : 
		StringMap(0),
		DoubleMap(0),
		IntMap(0),
		DictMutex()
	{
		ReadConfig(Filename_.c_str(),this);
	}
	~Dict() = default;
	
	/** @brief Copy constructor */
	explicit Dict(const Dict &D) : 
		StringMap(D.StringMap),
		DoubleMap(D.DoubleMap),
		IntMap(D.IntMap),
		DictMutex(),
		Filename(D.Filename) {}
	
	/** @brief Assignment operator */
	Dict& operator=(const Dict &D)
	{
		if (this != &D) {
			//Lock both mutexes when ready
			std::lock(DictMutex,D.DictMutex);
			std::lock_guard<std::mutex> L_local(DictMutex, std::adopt_lock);
			std::lock_guard<std::mutex> L_D(DictMutex, std::adopt_lock);
			StringMap = D.StringMap;
			DoubleMap = D.DoubleMap;
			IntMap = D.IntMap;
			Filename = D.Filename;
		}
		return *this;
	}
	
	/** @brief Add a `double` to the dictionary map */
	bool add(std::string const &key, double val);
	/** @overload bool add(std::string key, int val); */
	bool add(std::string const &key, int val);
	/** @overload bool add(std::string key, std::string val); */
	bool add(std::string const &key, std::string const &val);

	/** @brief Get the double corresponding to `key` */
	double GetDouble(std::string const &key) const;
	/** @brief Get the int corresponding to `key` */
	int GetInt(std::string const &key) const;
	/** @brief Get the string corresponding to `key` */
	std::string GetString(std::string const &key) const;
	/** @brief Get a boolean from strings map */
	bool GetBool(std::string const &key) const;
	
	/** @brief Check if `key` exists in `double`s map */
	bool CheckDouble(std::string const &key) const;
	/** @brief Check if `key` exists in `int`s map*/
	bool CheckInt(std::string const &key) const;
	/** @brief Check if `key` exists in `std::string`s map */
	bool CheckString(std::string const &key) const;

	/** @brief Prints information about loaded dictionary to stdout */
	void Dump() const;
};
/** @} */

} //namespace ParseLi

#endif
