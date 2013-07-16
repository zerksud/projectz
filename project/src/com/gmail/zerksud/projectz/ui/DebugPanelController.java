package com.gmail.zerksud.projectz.ui;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;

import com.gmail.zerksud.common.NotificationCenter;
import com.gmail.zerksud.projectz.R;

public class DebugPanelController {

    public DebugPanelController(Context context, ViewGroup parentViewGroup) {
        View v = LayoutInflater.from(context).inflate(R.layout.debug_panel, parentViewGroup, false);
        parentViewGroup.addView(v);

        setupListeners(v);
    }

    private void setupListeners(View v) {
        final View toggleWireFrameBtn = (View) v.findViewById(R.id.toggle_wireframe_button);
        toggleWireFrameBtn.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                NotificationCenter.getInstance().postNotification("UIDebugToggleWireframe");
            }
        });
    }
}
