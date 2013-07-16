package com.gmail.zerksud.projectz.general;

import org.json.JSONException;
import org.json.JSONObject;

import com.gmail.zerksud.common.NotificationCenter;
import com.gmail.zerksud.projectz.R;
import com.gmail.zerksud.projectz.ui.UIManager;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity implements SurfaceHolder.Callback {

    private UIManager mUIManager = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i("ZLogger", "MainActivity.onCreate");
        super.onCreate(savedInstanceState);

        NativeProxy.getInstance().setMainActivity(this);
        NotificationCenter.getInstance().postNotification("ActivityOnCreate");

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.main);

        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.gl_surface);
        surfaceView.getHolder().addCallback(this);

        mUIManager = new UIManager(this);
    }

    @Override
    protected void onResume() {
    	Log.i("ZLogger", "MainActivity.onResume");
        super.onResume();
        NotificationCenter.getInstance().postNotification("ActivityOnResume");
    }

    @Override
    protected void onPause() {
    	Log.i("ZLogger", "MainActivity.onPause");
        super.onPause();
        NotificationCenter.getInstance().postNotification("ActivityOnPause");
    }

    @Override
    protected void onStop() {
    	Log.i("ZLogger", "MainActivity.onStop");
        super.onStop();
        NotificationCenter.getInstance().postNotification("ActivityOnStop");
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.i("ZLogger", "MainActivity.surfaceChanged");
        try {
            JSONObject dict = new JSONObject();
            dict.put("width", width);
            dict.put("height", height);
            NotificationCenter.getInstance().postNotification("SurfaceSizeChanged", dict);
        } catch (JSONException e) {
            Log.e("ZLogger", "Can't create dict for surfaceChanged " + width + "x" + height);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i("ZLogger", "MainActivity.surfaceCreated");
        NativeProxy.getInstance().nativeSetSurface(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i("ZLogger", "MainActivity.surfaceDestroyed");
        NotificationCenter.getInstance().postNotification("SurfaceDestroyed");
    }
}
