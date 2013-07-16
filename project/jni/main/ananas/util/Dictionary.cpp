/*
 * Dictionary.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: zerksud
 */

#include "Dictionary.h"

#include "ZLogger.h"

namespace ann {
namespace util {

std::string dictionaryToString(const Dictionary& dict) {
    Json::FastWriter writer;
    return writer.write(dict);
}

Dictionary parseDictionary(const std::string& dictString) {
    Json::Reader reader;
    Dictionary dict;
    bool success = reader.parse(dictString, dict, false);

    if (!success) {
        LOGE("Can't parse JSON from %s", dictString.c_str());
    }

    return dict;
}

} /* namespace util */
} /* namespace ann */
