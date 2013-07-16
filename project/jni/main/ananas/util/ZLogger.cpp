/*
 * ZLogger.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: zerksud
 */

#include "ZLogger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "StringHelpers.h"

#define DEFAULT_TAG_PREFIX  "ZLogger"
#define DEFAULT_TAG_FORMAT  "%1$s [%2$s:%3$d]"

namespace ann {
namespace util {

ZLogger::ZLogger() {
    mTagPrefix = nullptr;
    setTagPrefix(DEFAULT_TAG_PREFIX);

    mTagFormat = nullptr;
    setTagFormat(DEFAULT_TAG_FORMAT);
}

ZLogger::~ZLogger() {
    delete[] mTagPrefix;
    delete[] mTagFormat;
}

void ZLogger::setTagPrefix(const char* tag) {
    ann::util::newStrCpy(&mTagPrefix, tag);
}

void ZLogger::setTagFormat(const char* format) {
    ann::util::newStrCpy(&mTagFormat, format);
}

void ZLogger::print(int priority, const char* fileName, int lineNum, const char* funcName, const char* format, ...) const {
    va_list args;
    va_start(args, format);
    int messageSize = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    char* userMessage = new char[messageSize + 1];
    va_start(args, format);
    vsprintf(userMessage, format, args);
    va_end(args);

    int logTagSize = snprintf(nullptr, 0, mTagFormat, mTagPrefix, fileName, lineNum, funcName);
    char* logTag = new char[logTagSize + 1];
    sprintf(logTag, mTagFormat, mTagPrefix, fileName, lineNum, funcName);

    __android_log_print(priority, logTag, userMessage);

    delete[] userMessage;
    delete[] logTag;
}

} /* namespace util */
} /* namespace ann */
