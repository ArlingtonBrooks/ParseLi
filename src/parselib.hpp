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

#include <string>
#include <unordered_map> //std::unordered_map

/** @class Dict
 * @brief A class contining information loaded from a configuration file
 * 
 * This class creates a set of unordered maps which translate a `std::string` key
 * 	into `double`, `int`, or `std::string` values read from an input
 *	configuration file.  
*/
class Dict
{
    std::unordered_map<std::string,double> DoubleMap;
    std::unordered_map<std::string,int> IntMap; 
    std::unordered_map<std::string,std::string> StringMap;

    public:
    /** @fn bool add(std::string key, double val);
     * @brief Add a `double` to the dictionary map
     * @param key Lookup value corresponding to `val`
     * @param val Value added with key `key`
     * @return `True` if successful, `False` otherwise
     */
    bool add(std::string key, double val);
    /** @overload bool add(std::string key, int val); */
    bool add(std::string key, int val);
    /** @overload bool add(std::string key, std::string val); */
    bool add(std::string key, std::string val);

    /** @fn double GetDouble(std::string key);
     * @brief Get the double corresponding to `key`
     * @param key Value to look up
     * @return Value corresponding to `key`
     * @throw std::out_of_range if value cannot be found
     */
    double GetDouble(std::string key);
    /** @fn double GetInt(std::string key);
     * @brief Get the int corresponding to `key`
     * @param key Value to look up
     * @return Value corresponding to `key`
     * @throw std::out_of_range if value cannot be found
     */
    int GetInt(std::string key);
    /** @fn double GetString(std::string key);
     * @brief Get the string corresponding to `key`
     * @param key Value to look up
     * @return Value corresponding to `key`
     * @throw std::out_of_range if value cannot be found
     */
    std::string GetString(std::string key);
    
    /** @fn bool CheckDouble(std::string key);
     * @brief Check if `key` exists in `double`s map
     * @param key Value to look up
     * @return `True` if `key` exists, `False` otherwise
     */
    bool CheckDouble(std::string key);
    /** @fn bool CheckInt(std::string key);
     * @brief Check if `key` exists in `int`s map
     * @param key Value to look up
     * @return `True` if `key` exists, `False` otherwise
     */
    bool CheckInt(std::string key);
    /** @fn bool CheckString(std::string key);
     * @brief Check if `key` exists in `std::string`s map
     * @param key Value to look up
     * @return `True` if `key` exists, `False` otherwise
     */
    bool CheckString(std::string key);

    /** @fn void Dump();
     * @brief Prints information about loaded dictionary to stdout
     */
    void Dump();
};

/** @fn bool ReadConfig(const char* filename, Dict* D, bool DEBUG);
 * @brief Reads a configuration file into a Dict pointer
 * 
 * @param filename Name of the configuration file to read
 * @param D Dictionary where information is loaded to
 * @param DEBUG Whether to print debugging information
 * @return `True` on successful read of config file
 *
 * @note The following code block represents an example input file
 * 	which will produce a collection of `int`s, `double`s, and
 *	`std::string`s.
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
        MAXITER 100000;
 * @endcode
 * @internal
*/
bool ReadConfig(const char* filename, Dict* D, bool DEBUG);

#endif
