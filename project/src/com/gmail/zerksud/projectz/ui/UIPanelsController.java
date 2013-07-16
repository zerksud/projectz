package com.gmail.zerksud.projectz.ui;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.gmail.zerksud.common.ButtonOnTouchListener;
import com.gmail.zerksud.projectz.R;

public class UIPanelsController {

    public UIPanelsController(Context context, ViewGroup parentViewGroup) {
        LayoutInflater inflater = LayoutInflater.from(context);

        View navigationPanel = inflater.inflate(R.layout.navigation_panel, parentViewGroup, false);
        parentViewGroup.addView(navigationPanel);
        setupNavigationListeners(navigationPanel);
    }

    private void setupNavigationListeners(View panel) {
        setTouchAction(panel, R.id.forward_button, "UIActionPlayerMoveForward");
        setTouchAction(panel, R.id.back_button, "UIActionPlayerMoveBackward");
        setTouchAction(panel, R.id.rotate_left_button, "UIActionPlayerRotateLeft");
        setTouchAction(panel, R.id.rotate_right_button, "UIActionPlayerRotateRight");
        setTouchAction(panel, R.id.move_left_button, "UIActionPlayerMoveLeft");
        setTouchAction(panel, R.id.move_right_button, "UIActionPlayerMoveRight");
    }

    private void setTouchAction(View parentView, int id, String action) {
        View touchableView = (View) parentView.findViewById(id);
        touchableView.setOnTouchListener(new ButtonOnTouchListener(action));
    }
}
