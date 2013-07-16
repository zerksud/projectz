/*
 * NotificationCenter.h
 *
 *  Created on: Mar 31, 2013
 *      Author: zerksud
 */

#ifndef NOTIFICATIONCENTER_H_
#define NOTIFICATIONCENTER_H_

#include <functional>
#include <unordered_map>
#include <vector>

#include "Dictionary.h"

namespace ann {
namespace util {

typedef std::function<void(const Dictionary&)> ObserverHandle;
typedef std::function<void(const std::string&, const Dictionary&)> NotificationProxyHandle;

class NotificationCenter {
public:
    static NotificationCenter& getInstance();

    void addObserver(const std::string& name, void* observerOwner, ObserverHandle observerHandle);
    void removeObserver(const std::string& name, void* observerOwner);

    void addProxy(void* proxyOwner, NotificationProxyHandle proxyHandle);
    void removeProxy(void* proxyOwner);  // TODO: const void* ?

    void postNotification(const std::string& name);
    void postNotification(const std::string& name, const Dictionary& dictionary);
    void postProxiedNotification(const std::string& name, const Dictionary& dictionary);

private:
    NotificationCenter();
    NotificationCenter(const NotificationCenter& other);
    NotificationCenter& operator=(const NotificationCenter& other);
    virtual ~NotificationCenter();

    void postNotificationToObservers(const std::string& name, const Dictionary& dictionary);
    void postNotificationToProxies(const std::string& name, const Dictionary& dictionary);

    struct Observer {
        void* observerOwner;
        ObserverHandle handle;

        Observer(void* observerPtr, ObserverHandle handlePtr) {
            observerOwner = observerPtr;
            handle = handlePtr;
        }
    };

    typedef std::vector<Observer> ObserverList;
    typedef std::unordered_map<std::string, ObserverList*> ObserverListTable;
    typedef std::pair<std::string, ObserverList*> ObserverListTablePair;

    ObserverListTable mObservers;

    struct Proxy {
        void* proxyOwner;
        NotificationProxyHandle handle;

        Proxy(void* proxyPtr, NotificationProxyHandle handlePtr) {
            proxyPtr = proxyPtr;
            handle = handlePtr;
        }
    };

    typedef std::vector<Proxy> ProxyList;

    ProxyList mProxies;
};

} /* namespace util */
} /* namespace ann */
#endif /* NOTIFICATIONCENTER_H_ */
