package me.simple.lib.glview;

import android.opengl.GLDebugHelper;
import android.util.Log;

import java.io.Writer;
import java.lang.ref.WeakReference;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;


/**
 * An EGL helper class.
 */

public class EglHelper {

    private WeakReference<GLView> mGLSurfaceViewWeakRef;
    EGL10 mEgl;
    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLSurface mRecorderEglSurface;
    EGLSurface mImageReaderEglSurface;
    EGLConfig mEglConfig;
    EGLContext mEglContext;

    public EglHelper(WeakReference<GLView> glSurfaceViewWeakRef) {
        mGLSurfaceViewWeakRef = glSurfaceViewWeakRef;
    }

    /**
     * Initialize EGL for a given configuration spec.
     *
     * @param
     */
    public void start() {
        /*
         * Get an EGL instance
         */
        mEgl = (EGL10) EGLContext.getEGL();

        /*
         * Get to the default display.
         */
        mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

        if (mEglDisplay == EGL10.EGL_NO_DISPLAY) {
            throw new RuntimeException("eglGetDisplay failed");
        }

        /*
         * We can now initialize EGL for that display
         */
        int[] version = new int[2];
        if (!mEgl.eglInitialize(mEglDisplay, version)) {
            throw new RuntimeException("eglInitialize failed");
        }
        GLView view = mGLSurfaceViewWeakRef.get();
        if (view == null) {
            mEglConfig = null;
            mEglContext = null;
        } else {
            mEglConfig = view.mEGLConfigChooser.chooseConfig(mEgl, mEglDisplay);

            /*
             * Create an EGL context. We want to do this as rarely as we can, because an
             * EGL context is a somewhat heavy object.
             */
            mEglContext = view.mEGLContextFactory.createContext(mEgl, mEglDisplay, mEglConfig);
        }
        if (mEglContext == null || mEglContext == EGL10.EGL_NO_CONTEXT) {
            mEglContext = null;
            throwEglException("createContext");
        }

        mEglSurface = null;
        mRecorderEglSurface = null;
        mImageReaderEglSurface = null;
    }

    /**
     * Create an egl surface for the current SurfaceHolder surface. If a surface
     * already exists, destroy it before creating the new surface.
     *
     * @return true if the surface was created successfully.
     */
    public boolean createSurface() {
        /*
         * Check preconditions.
         */
        if (mEgl == null) {
            throw new RuntimeException("egl not initialized");
        }
        if (mEglDisplay == null) {
            throw new RuntimeException("eglDisplay not initialized");
        }
        if (mEglConfig == null) {
            throw new RuntimeException("mEglConfig not initialized");
        }

        /*
         *  The window size has changed, so we need to create a new
         *  surface.
         */
        destroySurfaceImp();

        /*
         * Create an EGL surface we can render into.
         */
        GLView view = mGLSurfaceViewWeakRef.get();
        if (view != null) {
            mEglSurface = view.mEGLWindowSurfaceFactory.createWindowSurface(mEgl,
                    mEglDisplay, mEglConfig, view.getSurfaceTexture());
        } else {
            mEglSurface = null;
        }

        if (mEglSurface == null || mEglSurface == EGL10.EGL_NO_SURFACE) {
            int error = mEgl.eglGetError();
            if (error == EGL10.EGL_BAD_NATIVE_WINDOW) {
                Log.e("EglHelper", "createWindowSurface returned EGL_BAD_NATIVE_WINDOW.");
            }
            return false;
        }

       /* *//*
         * Before we can issue GL commands, we need to make sure
         * the context is current and bound to a surface.
         *//*
        if (!mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
            *//*
             * Could not make the context current, probably because the underlying
             * SurfaceView surface has been destroyed.
             *//*
            logEglErrorAsWarning("EGLHelper", "eglMakeCurrent", mEgl.eglGetError());
            return false;
        }
*/
        return true;
    }

    public boolean createRecordableSurface() {
        /*
         * Check preconditions.
         */
        if (mEgl == null) {
            throw new RuntimeException("egl not initialized");
        }
        if (mEglDisplay == null) {
            throw new RuntimeException("eglDisplay not initialized");
        }
        if (mEglConfig == null) {
            throw new RuntimeException("mEglConfig not initialized");
        }

        destroyRecorderSurface();
        GLView view = mGLSurfaceViewWeakRef.get();
        if (view != null) {
            mRecorderEglSurface = view.mEGLRecordableSurfaceFactory.createWindowSurface(mEgl,
                    mEglDisplay, mEglConfig, null);
        } else {
            mRecorderEglSurface = null;
        }

        if (mRecorderEglSurface == null || mRecorderEglSurface == EGL10.EGL_NO_SURFACE) {
            int error = mEgl.eglGetError();
            if (error == EGL10.EGL_BAD_NATIVE_WINDOW) {
                Log.e("EglHelper", "createWindowSurface returned EGL_BAD_NATIVE_WINDOW.");
            }
            return false;
        }

        return true;
    }

    public boolean createImageReaderSurface() {

        /*
         * Check preconditions.
         */
        if (mEgl == null) {
            throw new RuntimeException("egl not initialized");
        }
        if (mEglDisplay == null) {
            throw new RuntimeException("eglDisplay not initialized");
        }
        if (mEglConfig == null) {
            throw new RuntimeException("mEglConfig not initialized");
        }

        destroyRecorderSurface();
        GLView view = mGLSurfaceViewWeakRef.get();
        if (view != null) {
            mImageReaderEglSurface = view.mEGLImageReaderSurfaceFactory.createWindowSurface(mEgl,
                    mEglDisplay, mEglConfig, null);
        } else {
            mImageReaderEglSurface = null;
        }

        if (mImageReaderEglSurface == null || mImageReaderEglSurface == EGL10.EGL_NO_SURFACE) {
            int error = mEgl.eglGetError();
            if (error == EGL10.EGL_BAD_NATIVE_WINDOW) {
                Log.e("EglHelper", "createWindowSurface returned EGL_BAD_NATIVE_WINDOW.");
            }
            return false;
        }

        return true;
    }

    /**
     * Create a GL object for the current EGL context.
     *
     * @return
     */
    GL createGL() {

        GL gl = mEglContext.getGL();
        GLView view = mGLSurfaceViewWeakRef.get();
        if (view != null) {
            if (view.mGLWrapper != null) {
                gl = view.mGLWrapper.wrap(gl);
            }

            if ((view.mDebugFlags & (GLView.DEBUG_CHECK_GL_ERROR | GLView.DEBUG_LOG_GL_CALLS)) != 0) {
                int configFlags = 0;
                Writer log = null;
                if ((view.mDebugFlags & GLView.DEBUG_CHECK_GL_ERROR) != 0) {
                    configFlags |= GLDebugHelper.CONFIG_CHECK_GL_ERROR;
                }
                if ((view.mDebugFlags & GLView.DEBUG_LOG_GL_CALLS) != 0) {
                    log = new LogWriter();
                }
                gl = GLDebugHelper.wrap(gl, configFlags, log);
            }
        }
        return gl;
    }

    public boolean makeCurrent(int currentSurfaceIndex) {
        EGLSurface currentSurface;
        switch(currentSurfaceIndex) {
            case 1:
                currentSurface = mRecorderEglSurface;
                break;
            case 2:
                currentSurface = mImageReaderEglSurface;
                break;
            default:
                currentSurface = mEglSurface;
                break;
        }
        if (currentSurface == null) {
            return false;
        }
        if (!mEgl.eglMakeCurrent(mEglDisplay, currentSurface, currentSurface, mEglContext)) {
            return false;
        }
        return true;
    }

    /**
     * Display the current render surface.
     *
     * @return the EGL error code from eglSwapBuffers.
     */
    public int swap() {
        if (!mEgl.eglSwapBuffers(mEglDisplay, mEglSurface)) {
            return mEgl.eglGetError();
        }
        return EGL10.EGL_SUCCESS;
    }

    public int swap(int index) {
        EGLSurface surface;
        switch (index)
        {
            case 1:
                surface = mRecorderEglSurface;
                break;
            case 2:
                surface = mImageReaderEglSurface;
                break;
            default:
                surface = mEglSurface;
                break;
        }
        if (!mEgl.eglSwapBuffers(mEglDisplay, surface)) {
            return mEgl.eglGetError();
        }
        return EGL10.EGL_SUCCESS;
    }

    public void destroySurface() {
        destroySurfaceImp();
    }

    private void destroySurfaceImp() {
        if (mEglSurface != null && mEglSurface != EGL10.EGL_NO_SURFACE) {
            mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
                    EGL10.EGL_NO_SURFACE,
                    EGL10.EGL_NO_CONTEXT);
            GLView view = mGLSurfaceViewWeakRef.get();
            if (view != null) {
                view.mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mEglSurface);
            }
            mEglSurface = null;
        }
    }

    public  void destroyRecorderSurface() {
        if (mRecorderEglSurface != null && mRecorderEglSurface != EGL10.EGL_NO_SURFACE) {
            GLView view = mGLSurfaceViewWeakRef.get();
            if (view != null) {
                view.mEGLRecordableSurfaceFactory.destroySurface(mEgl, mEglDisplay, mRecorderEglSurface);
            }
            mRecorderEglSurface = null;
        }
    }

    public  void destroyImageReaderSurface() {
        if (mImageReaderEglSurface != null && mImageReaderEglSurface != EGL10.EGL_NO_SURFACE) {
            GLView view = mGLSurfaceViewWeakRef.get();
            if (view != null) {
                view.mEGLWindowSurfaceFactory.destroySurface(mEgl, mEglDisplay, mImageReaderEglSurface);
            }
            mImageReaderEglSurface = null;
        }
    }

    public void finish() {
        if (mEglContext != null) {
            GLView view = mGLSurfaceViewWeakRef.get();
            if (view != null) {
                view.mEGLContextFactory.destroyContext(mEgl, mEglDisplay, mEglContext);
            }
            mEglContext = null;
        }
        if (mEglDisplay != null) {
            mEgl.eglTerminate(mEglDisplay);
            mEglDisplay = null;
        }
    }

    private void throwEglException(String function) {
        throwEglException(function, mEgl.eglGetError());
    }

    public static void throwEglException(String function, int error) {
        String message = formatEglError(function, error);
        throw new RuntimeException(message);
    }

    public static String formatEglError(String function, int error) {
        return function + " failed: " + error;
    }

}