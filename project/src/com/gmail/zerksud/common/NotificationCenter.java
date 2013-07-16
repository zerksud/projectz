package com.gmail.zerksud.common;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;

import android.util.Log;

public final class NotificationCenter {

    private Map<String, List<WeakReference<NotificationObserver>>> mObservers;
    private List<WeakReference<NotificationProxy>> mProxies;

    private NotificationCenter() {
        mObservers = new HashMap<String, List<WeakReference<NotificationObserver>>>();
        mProxies = new ArrayList<WeakReference<NotificationProxy>>();
    }

    private static NotificationCenter sInstance;

    public static NotificationCenter getInstance() {
        if (sInstance == null) {
            sInstance = new NotificationCenter();
        }

        return sInstance;
    }

    private void postNotificationToProxies(String notificationName, JSONObject dictionary) {
        for (WeakReference<NotificationProxy> weakProxy : mProxies) {
            NotificationProxy referentProxy = weakProxy.get();
            if (referentProxy != null) {
                referentProxy.proxyNotification(notificationName, dictionary);
            }
        }
    }

    private void postNotificationToObservers(String notificationName, JSONObject dictionary) {
        List<WeakReference<NotificationObserver>> observers = mObservers.get(notificationName);
        if (observers == null) {
            return;
        }

        for (WeakReference<NotificationObserver> weakObserver : observers) {
            NotificationObserver referentObserver = weakObserver.get();
            if (referentObserver == null) {
                continue;
            }

            referentObserver.handleNotification(dictionary);
        }
    }

    public void postNotification(String notificationName) {
        postNotification(notificationName, new JSONObject());
    }

    public void postNotification(String notificationName, JSONObject dictionary) {
        postNotificationToObservers(notificationName, dictionary);
        postNotificationToProxies(notificationName, dictionary);
    }

    public void postProxiedNotification(String notificationName, JSONObject dictionary) {
        postNotificationToObservers(notificationName, dictionary);
    }

    public void addObserver(String notificationName, NotificationObserver observer) {
        List<WeakReference<NotificationObserver>> observers = mObservers.get(notificationName);
        if (observers == null) {
            observers = new ArrayList<WeakReference<NotificationObserver>>();
            mObservers.put(notificationName, observers);
        }
        observers.add(new WeakReference<NotificationObserver>(observer));
    }

    public void removeObserver(String notificationName, NotificationObserver observer) {
        List<WeakReference<NotificationObserver>> observers = mObservers.get(notificationName);
        if (observers != null) {
            List<WeakReference<NotificationObserver>> weakObservers2Remove = new ArrayList<WeakReference<NotificationObserver>>();
            for (WeakReference<NotificationObserver> weakObserver : observers) {
                NotificationObserver referentObserver = weakObserver.get();
                if (referentObserver == observer || referentObserver == null) {
                    weakObservers2Remove.add(weakObserver);
                }
            }

            if (weakObservers2Remove.size() != observers.size()) {
                observers.removeAll(weakObservers2Remove);
            } else {
                mObservers.remove(notificationName);
            }
        }
    }

    public void addProxy(NotificationProxy proxy) {
        for (WeakReference<NotificationProxy> weakProxy : mProxies) {
            NotificationProxy referentProxy = weakProxy.get();
            if (referentProxy == proxy) {
                Log.e("ZLogger", "Can't add already added proxy");
                return;
            }
        }

        mProxies.add(new WeakReference<NotificationProxy>(proxy));
    }

    public void removeProxy(NotificationProxy proxy) {
        List<WeakReference<NotificationProxy>> weakProxies2Remove = new ArrayList<WeakReference<NotificationProxy>>();
        for (WeakReference<NotificationProxy> weakProxy : mProxies) {
            NotificationProxy referentProxy = weakProxy.get();
            if (referentProxy == proxy || referentProxy == null) {
                weakProxies2Remove.add(weakProxy);
            }
        }

        if (weakProxies2Remove.size() != mProxies.size()) {
            mProxies.removeAll(weakProxies2Remove);
        } else {
            mProxies.clear();
        }
    }

}
