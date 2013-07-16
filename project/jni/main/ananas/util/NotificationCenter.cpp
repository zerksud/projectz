/*
 * NotificationCenter.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: zerksud
 */

#include "NotificationCenter.h"

#include <algorithm>

#include "ZLogger.h"

namespace ann {
namespace util {

NotificationCenter& NotificationCenter::getInstance() {
    static NotificationCenter instance;
    return instance;
}

NotificationCenter::NotificationCenter() {
}

NotificationCenter::~NotificationCenter() {
    for (const ObserverListTablePair& e : mObservers) {
        ObserverList* list = e.second;
        delete list;
    }
}

void NotificationCenter::addObserver(const std::string& name, void* observerOwner, ObserverHandle observerHandle) {
    ObserverList* list;
    ObserverListTable::iterator pos = mObservers.find(name);
    if (pos == mObservers.end()) {
        list = new ObserverList();
        mObservers[name] = list;
    } else {
        list = pos->second;
    }

    ObserverList::const_iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const Observer& handle) {
        return (observerOwner == handle.observerOwner);
    });

    if (listPos != list->end()) {
        LOGE("Can't add already added observer for %s", name.c_str());
        return;
    }

    list->push_back(Observer(observerOwner, observerHandle));
}

void NotificationCenter::removeObserver(const std::string& name, void* observerOwner) {
    ObserverList* list;
    ObserverListTable::iterator pos = mObservers.find(name);
    if (pos == mObservers.end()) {
        LOGE("Can't remove not added observer for %s", name.c_str());
        return;
    } else {
        list = pos->second;
    }

    ObserverList::iterator listPos = std::find_if(list->begin(), list->end(), [observerOwner](const Observer& handle) {
        return (observerOwner == handle.observerOwner);
    });

    if (listPos == list->end()) {
        LOGE("Can't remove not added observer for %s", name.c_str());
        return;
    }

    list->erase(listPos);
}

void NotificationCenter::addProxy(void* proxyOwner, NotificationProxyHandle proxyHandle) {
    ProxyList::const_iterator proxyPos = std::find_if(mProxies.begin(), mProxies.end(), [proxyOwner](const Proxy& proxyBox) {
        return (proxyOwner == proxyBox.proxyOwner);
    });

    if (proxyPos != mProxies.end()) {
        LOGE("Can't add already added proxy");
        return;
    }

    mProxies.push_back(Proxy(proxyOwner, proxyHandle));
}

void NotificationCenter::removeProxy(void* proxyOwner) {
    ProxyList::iterator proxyPos = std::find_if(mProxies.begin(), mProxies.end(), [proxyOwner](const Proxy& proxyBox) {
        return (proxyOwner == proxyBox.proxyOwner);
    });

    if (proxyPos == mProxies.end()) {
        LOGE("Can't remove not added proxy");
        return;
    }

    mProxies.erase(proxyPos);
}

void NotificationCenter::postProxiedNotification(const std::string& name, const Dictionary& dictionary) {
    postNotificationToObservers(name, dictionary);
}

void NotificationCenter::postNotification(const std::string& name) {
    postNotification(name, Dictionary(Json::nullValue));
}

void NotificationCenter::postNotification(const std::string& name, const Dictionary& dictionary) {
    postNotificationToObservers(name, dictionary);
    postNotificationToProxies(name, dictionary);
}

void NotificationCenter::postNotificationToProxies(const std::string& name, const Dictionary& dictionary) {
    for (Proxy& p : mProxies) {
        p.handle(name, dictionary);
    }
}

void NotificationCenter::postNotificationToObservers(const std::string& name, const Dictionary& dictionary) {
    ObserverListTable::iterator pos = mObservers.find(name);
    if (pos != mObservers.end()) {
        ObserverList* list = pos->second;
        for (Observer& observer : *list) {
            observer.handle(dictionary);
        }
    }
}

} /* namespace util */
} /* namespace ann */
