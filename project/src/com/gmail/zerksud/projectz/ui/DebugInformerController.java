package com.gmail.zerksud.projectz.ui;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.TextView;

import com.gmail.zerksud.common.NotificationCenter;
import com.gmail.zerksud.common.NotificationObserver;
import com.gmail.zerksud.projectz.R;

public class DebugInformerController {
    public static final String DEBUG_VALUE_NOTIFICATION = "debugValueChanged";

    public static class DebugValue {
        private String mName;
        private String mValue;

        public DebugValue(String name, String value) {
            mName = name;
            mValue = value;
        }

        public String getName() {
            return mName;
        }

        public String getValue() {
            return mValue;
        }
    }

    private NotificationObserver mUpdateDebugObserver;
    private Map<String, String> mValues;
    private TextView mTextView;

    public DebugInformerController(Context context, ViewGroup parentViewGroup) {
        mTextView = (TextView) LayoutInflater.from(context).inflate(R.layout.debug_informer, parentViewGroup, false);
        parentViewGroup.addView(mTextView);

        mValues = new HashMap<String, String>();
        setupObserver();
    }

    private void setupObserver() {
        mUpdateDebugObserver = new NotificationObserver() {

            @Override
            public void handleNotification(JSONObject dictionary) {
                try {
                    String name = dictionary.getString("name");
                    String value = dictionary.getString("value");
                    mValues.put(name, value);
                    updateValues();

                } catch (JSONException e) {
                    Log.e("ZLogger", "Can't extract name-value pair from dictionary " + dictionary.toString());
                }
            }
        };

        NotificationCenter.getInstance().addObserver(DEBUG_VALUE_NOTIFICATION, mUpdateDebugObserver);
    }

    private void updateValues() {
        StringBuilder sb = new StringBuilder();
        for (String name : mValues.keySet()) {
            String value = mValues.get(name);
            sb.append(String.format("%s: %s\n", name, value));
        }
        mTextView.setText(sb.toString());
    }

}
