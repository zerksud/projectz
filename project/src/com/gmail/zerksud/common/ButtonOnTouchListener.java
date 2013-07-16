package com.gmail.zerksud.common;

import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class ButtonOnTouchListener implements OnTouchListener {
    private final Runnable mAction;

    public ButtonOnTouchListener(Runnable action) {
        mAction = action;
    }

    public ButtonOnTouchListener(String notificationName) {
        mAction = new NotificationRunnable(notificationName);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getAction();
        switch (action) {
        case MotionEvent.ACTION_DOWN:
            v.setPressed(true);
            mAction.run();
            return true;
        default:
            return false;
        }
    }
}
