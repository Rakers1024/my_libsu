package com.kmxs.reader.utils;

import android.animation.ObjectAnimator;
import android.view.View;

public class AnimatorUtils {

    public static void alpha(View view,float[] arry,Long time){
        ObjectAnimator.ofFloat(view,"alpha",arry).setDuration(time).start();
    }

    public static void custom(View view, String propertyName, float[] arry,Long time){
        ObjectAnimator.ofFloat(view,propertyName,arry).setDuration(time).start();
    }
}
