package com.gmail.zerksud.projectz.general;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.res.AssetManager;
import android.util.Log;
import android.view.Surface;

import com.gmail.zerksud.common.NotificationCenter;
import com.gmail.zerksud.common.NotificationProxy;

public final class NativeProxy {
    static {
        System.loadLibrary("main");
    }

    private static NativeProxy sInstance;

    public static NativeProxy getInstance() {
        if (sInstance == null) {
            sInstance = new NativeProxy();
        }

        return sInstance;
    }

    private NotificationProxy mNotificationProxy;
    private Activity mMainActivity;

    private NativeProxy() {
        initializeNotificationProxy();
        nativeSetNativeProxy();
    };

    private native void nativeSetNativeProxy();

    public void setMainActivity(Activity mainActivity) {
        mMainActivity = mainActivity;
        nativeSetAssetManager(mainActivity.getAssets());
    }

    public native void nativeSetSurface(Surface surface);

    private native void nativeSetAssetManager(AssetManager assetManager);

    private native void nativeProxyNotification(String name, String dictionary);

    private void initializeNotificationProxy() {
        mNotificationProxy = new NotificationProxy() {

            @Override
            public void proxyNotification(String name, JSONObject dictionary) {
                Log.d("ZLogger", "proxyNotification " + name + ": " + dictionary.toString());
                NativeProxy.getInstance().nativeProxyNotification(name, dictionary.toString());
            }
        };
        NotificationCenter.getInstance().addProxy(mNotificationProxy);
    }

    public void postNotification(String name, String dict) {
        if (mMainActivity == null) {
            Log.e("ZLogger", "Can't post notification without Activity reference");
            return;
        }

        final String notificationName = name;
        try {
            final JSONObject jsonObject = new JSONObject(dict);

            mMainActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    NotificationCenter.getInstance().postProxiedNotification(notificationName, jsonObject);
                    // TODO: parse HashMap from JSON or maybe pass string to NotificationCenter and parse it inside center/observer if there is any?
                }
            });

        } catch (JSONException e) {
            Log.e("ZLogger", "Can't parse JSON from " + dict);
        }
    }

}
