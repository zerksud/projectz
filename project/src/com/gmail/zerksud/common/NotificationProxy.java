package com.gmail.zerksud.common;

import org.json.JSONObject;

public interface NotificationProxy {
    void proxyNotification(String name, JSONObject dictionary);
}
