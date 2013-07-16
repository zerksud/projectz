/*
 * NotificationProxy.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: zerksud
 */

#include "NotificationProxy.h"

#include <ananas/android/AndroidLayer.h>

namespace ann {

NotificationProxy::NotificationProxy() {
    util::NotificationCenter::getInstance().addProxy(this, [this](const std::string& name, const util::Dictionary& dict){
        this->exportNotification(name, dict);
    });
}

NotificationProxy::~NotificationProxy() {
}

void NotificationProxy::importNotification(const std::string& name, const std::string& dictString) {
    util::Dictionary dict = util::parseDictionary(dictString);

    ann::util::NotificationCenter::getInstance().postProxiedNotification(name, dict);
}

void NotificationProxy::exportNotification(const std::string& name, const util::Dictionary& dict) {
    std::string dictString = util::dictionaryToString(dict);
    android::AndroidLayer::getInstance().postNotification(name, dictString);
}

} /* namespace ann */
