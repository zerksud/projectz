/*
 * Dictionary.h
 *
 *  Created on: Apr 4, 2013
 *      Author: zerksud
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <json/json.h>

namespace ann {
namespace util {

typedef Json::Value Dictionary;

std::string dictionaryToString(const Dictionary& dict);
Dictionary parseDictionary(const std::string& dictString);

} /* namespace util */
} /* namespace ann */
#endif /* DICTIONARY_H_ */
