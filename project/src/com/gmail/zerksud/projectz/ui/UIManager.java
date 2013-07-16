package com.gmail.zerksud.projectz.ui;

import android.app.Activity;
import android.view.ViewGroup;

public class UIManager {
    private UIPanelsController mNavigationPanel;
    private DebugPanelController mDebugPanel;
    private DebugInformerController mDebugInformer;

    public UIManager(Activity mainActivity) {
        ViewGroup mainViewGroup = (ViewGroup) mainActivity.findViewById(android.R.id.content);
        mainViewGroup = (ViewGroup) mainViewGroup.getChildAt(0);

        mNavigationPanel = new UIPanelsController(mainActivity, mainViewGroup);
        mDebugPanel = new DebugPanelController(mainActivity, mainViewGroup);
        mDebugInformer = new DebugInformerController(mainActivity, mainViewGroup);
    }
}
