/*
 * StringHelpers.h
 *
 *  Created on: Feb 28, 2013
 *      Author: zerksud
 */

#ifndef STRINGHELPERS_H_
#define STRINGHELPERS_H_

#include <string>
#include <vector>

namespace ann {
namespace util {

/*
 * Copies the byte string pointed to by src to byte string, pointed to by dest.
 * Deallocates memory pointed by *dest != nullptr and allocates necessary memory to store src copy
 */
void newStrCpy(const char** dest, const char* src);

void trim(std::string* str);

std::string trim(const std::string& str);

std::vector<std::string> split(const std::string& str, char delim);

bool parseInt(const std::string& str, int* value);

std::string stringFormat(const char* format, ...);

} /* namespace util */
} /* namespace ann */
#endif /* STRINGHELPERS_H_ */
