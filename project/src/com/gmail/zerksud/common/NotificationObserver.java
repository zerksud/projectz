package com.gmail.zerksud.common;

import org.json.JSONObject;

public interface NotificationObserver {
    void handleNotification(JSONObject dictionary);
}
