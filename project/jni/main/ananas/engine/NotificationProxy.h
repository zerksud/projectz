/*
 * NotificationProxy.h
 *
 *  Created on: Apr 2, 2013
 *      Author: zerksud
 */

#ifndef NOTIFICATIONPROXY_H_
#define NOTIFICATIONPROXY_H_

#include <ananas/util/NotificationCenter.h>

namespace ann {

class NotificationProxy {
public:
    NotificationProxy();
    virtual ~NotificationProxy();

    void importNotification(const std::string& name, const std::string& dictString);

private:
    void exportNotification(const std::string& name, const util::Dictionary& dict);
};

} /* namespace ann */
#endif /* NOTIFICATIONPROXY_H_ */
