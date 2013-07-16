/*
 * CommonHelpers.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: zerksud
 */

#include "CommonHelpers.h"

#include "NotificationCenter.h"
#include "Constants.h"

namespace ann {
namespace util {
namespace helpers {

void logDebugValueChange(const std::string& name, const std::string& value) {
    util::Dictionary dict;
    dict["name"] = name;
    dict["value"] = value;
    NotificationCenter::getInstance().postNotification(Constants::kDebugValueNotification, dict);
}

} /* namespace helpers */
} /* namespace util */
} /* namespace ann */
