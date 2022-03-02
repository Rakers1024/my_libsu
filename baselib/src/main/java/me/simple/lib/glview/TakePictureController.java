package me.simple.lib.glview;

import android.annotation.SuppressLint;
import android.graphics.PixelFormat;
import android.media.Image;
import android.media.ImageReader;
import android.view.Surface;

public class TakePictureController {

    private ImageReader mImageReader;

    @SuppressLint("WrongConstant")
    public TakePictureController(int width, int height) {
        mImageReader = ImageReader.newInstance(width, height,
                PixelFormat.RGBA_8888,2);
        mImageReader.setOnImageAvailableListener(mOnImageAvailableListener, null);
    }

    public Surface getSurface() {
        return mImageReader.getSurface();
    }

    private final ImageReader.OnImageAvailableListener mOnImageAvailableListener
            = new ImageReader.OnImageAvailableListener() {

        @Override
        public void onImageAvailable(ImageReader reader) {
            Image image = reader.acquireNextImage();
            image.close();
        }

    };
}
