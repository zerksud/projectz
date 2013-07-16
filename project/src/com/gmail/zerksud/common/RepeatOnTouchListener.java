package com.gmail.zerksud.common;

import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class RepeatOnTouchListener implements OnTouchListener {

    private static class RepeatRunnable implements Runnable {
        private static final int REPEAT_ACTION_DELAY = 40;

        private Handler mHandler;
        private Runnable mAction;

        public RepeatRunnable(Runnable action, Handler handler) {
            mAction = action;
            mHandler = handler;
        }

        @Override
        public void run() {
            mAction.run();
            mHandler.postDelayed(this, REPEAT_ACTION_DELAY);
        }
    }

    private final RepeatRunnable mRepeatAction;
    private final Handler mHandler;

    public RepeatOnTouchListener(Runnable action) {
        mHandler = new Handler();
        mRepeatAction = new RepeatRunnable(action, mHandler);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getAction();
        switch (action) {
        case MotionEvent.ACTION_DOWN:
            v.setPressed(true);
            mRepeatAction.run();
            return true;
        case MotionEvent.ACTION_UP:
            v.setPressed(false);
            mHandler.removeCallbacks(mRepeatAction);
            return true;
        default:
            return false;
        }
    }

}
