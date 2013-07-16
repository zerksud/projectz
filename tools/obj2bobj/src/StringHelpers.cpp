/*
 * StringHelpers.cpp
 *
 *  Created on: Feb 28, 2013
 *      Author: zerksud
 */

#include "StringHelpers.h"

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include <sstream>

namespace ann {
namespace util {

void newStrCpy(const char** dest, const char* src) {
    char* buff = new char[strlen(src) + 1];
    ::strcpy(buff, src);

    if (*dest != nullptr) {
        delete[] *dest;
    }
    *dest = buff;
}

void trim(std::string* str) {
    std::string::size_type leftEdge = str->find_first_not_of(' ');
    if (leftEdge == std::string::npos) {
        *str = "";
        return;
    }

    std::string::size_type rightEdge = str->find_last_not_of(' ');
    std::string::size_type range = rightEdge - leftEdge + 1;

    *str = str->substr(leftEdge, range);
}

std::string trim(const std::string& str) {
    std::string copy = str;
    trim(&copy);
    return copy;
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> result;

    std::stringstream ss(str);
    std::string substring;
    while (std::getline(ss, substring, delim)) {
        result.push_back(substring);
    }

    return result;
}

bool parseInt(const std::string& str, int* value) {
    errno = 0;

    *value = ::atoi(str.c_str());

    bool success = false;
    if (errno == 0) {
        success = true;
    }
    errno = 0;

    return success;
}

std::string stringFormat(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int messageSize = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    char* message = new char[messageSize + 1];
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    std::string result = message;

    delete[] message;

    return result;
}

} /* namespace util */
} /* namespace ann */
