package me.simple.lib.utils;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;

import io.reactivex.Observer;
import io.reactivex.annotations.NonNull;
import io.reactivex.disposables.Disposable;
import me.simple.lib.net.RetrofitClient;

public class ImageUtils {


    public static void loadUrl(Context context, String url, ImageView view, Drawable error) {
        RetrofitClient.getInstance().downLoad(url, new Observer<byte[]>() {
            @Override
            public void onSubscribe(@NonNull Disposable d) {

            }

            @SuppressLint("UseCompatLoadingForDrawables")
            @Override
            public void onNext(@NonNull byte[] bytes) {
                if (bytes == null) {
                    view.setImageDrawable(error);
                } else {
                    Bitmap map = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
                    BitmapDrawable bd = new BitmapDrawable(context.getResources(), map);
                    view.setImageDrawable(bd);
                }

            }

            @SuppressLint("UseCompatLoadingForDrawables")
            @Override
            public void onError(@NonNull Throwable e) {
                view.setImageDrawable(error);
            }

            @Override
            public void onComplete() {

            }
        });
    }
}
