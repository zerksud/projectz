package com.gmail.zerksud.common;

public class NotificationRunnable implements Runnable {
    private final String mNotificationName;

    public NotificationRunnable(String notificationName) {
        mNotificationName = notificationName;
    }

    @Override
    public void run() {
        NotificationCenter.getInstance().postNotification(mNotificationName);
    }
}
