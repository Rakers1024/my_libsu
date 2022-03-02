package me.simple.lib.glview;



import java.lang.ref.WeakReference;
import java.util.ArrayList;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.opengles.GL10;


/**
 * A generic GL Thread. Takes care of initializing EGL and GL. Delegates
 * to a Renderer instance to do the actual drawing. Can be configured to
 * render continuously or on request.
 * <p>
 * All potentially blocking synchronization is done through the
 * GLView.sGLThreadManager object. This avoids multiple-lock ordering issues.
 */
public class GLThread extends Thread {

    // Once the thread is started, all accesses to the following member
    // variables are protected by the GLView.sGLThreadManager monitor
    private boolean mShouldExit;
    boolean mExited;
    private boolean mRequestPaused;
    private boolean mPaused;
    private boolean mHasSurface;
    private boolean mSurfaceIsBad;
    private boolean mWaitingForSurface;
    private boolean mHaveEglContext;
    private boolean mHaveEglSurface;
    private boolean mShouldReleaseEglContext;
    private int mWidth;
    private int mHeight;
    private int mRenderMode;
    private boolean mRequestRender;
    private boolean mRenderComplete;
    private ArrayList<Runnable> mEventQueue = new ArrayList<Runnable>();
    private boolean mSizeChanged = true;

    // End of member variables protected by the GLView.sGLThreadManager monitor.

    private EglHelper mEglHelper;

    /**
     * Set once at thread construction time, nulled out when the parent view is garbage
     * called. This weak reference allows the GLView to be garbage collected while
     * the GLThread is still alive.
     */
    private WeakReference<GLView> mGLSurfaceViewWeakRef;


    GLThread(WeakReference<GLView> glSurfaceViewWeakRef) {
        super();
        mWidth = 0;
        mHeight = 0;
        mRequestRender = true;
        mRenderMode = GLView.RENDERMODE_CONTINUOUSLY;
        mGLSurfaceViewWeakRef = glSurfaceViewWeakRef;
    }

    @Override
    public void run() {
        setName("GLThread " + getId());
        try {
            guardedRun();
        } catch (InterruptedException e) {
            // fall thru and exit normally
        } finally {
            GLView.sGLThreadManager.threadExiting(this);
        }
    }

    /*
     * This private method should only be called inside a
     * synchronized(GLView.sGLThreadManager) block.
     */
    private void stopEglSurfaceLocked() {
        if (mHaveEglSurface) {
            mHaveEglSurface = false;
            mEglHelper.destroySurface();
        }
    }

    /*
     * This private method should only be called inside a
     * synchronized(GLView.sGLThreadManager) block.
     */
    private void stopEglContextLocked() {
        if (mHaveEglContext) {
            mEglHelper.finish();
            mHaveEglContext = false;
            GLView.sGLThreadManager.releaseEglContextLocked(this);
        }
    }

    private void guardedRun() throws InterruptedException {
        mEglHelper = new EglHelper(mGLSurfaceViewWeakRef);
        mHaveEglContext = false;
        mHaveEglSurface = false;
        try {
            GL10 gl = null;
            boolean createEglContext = false;
            boolean createEglSurface = false;
            boolean createGlInterface = false;
            boolean lostEglContext = false;
            boolean sizeChanged = false;
            boolean wantRenderNotification = false;
            boolean doRenderNotification = false;
            boolean askedToReleaseEglContext = false;
            boolean createRecordableSurface = false;
            boolean createImageReaderSurface = false;
            int w = 0;
            int h = 0;
            Runnable event = null;

            while (true) {  /*外循环，画帧的循环，每一帧一个循环节*/
                synchronized (GLView.sGLThreadManager) {
                    while (true) {  /*内循环，准备画opengl的内容，如果不完整或者状态不对即会wait，等待下一个notif进入下一个循环。循环退出后进入准备循环opengl*/
                        if (mShouldExit) {
                            return;
                        }

                        if (!mEventQueue.isEmpty()) {
                            event = mEventQueue.remove(0);
                            break;
                        }

                        // Update the pause state.
                        boolean pausing = false;
                        if (mPaused != mRequestPaused) {
                            pausing = mRequestPaused;
                            mPaused = mRequestPaused;
                            GLView.sGLThreadManager.notifyAll();
                        }

                        // Do we need to give up the EGL context?
                        if (mShouldReleaseEglContext) {
                            stopEglSurfaceLocked();
                            stopEglContextLocked();
                            mShouldReleaseEglContext = false;
                            askedToReleaseEglContext = true;
                        }

                        // Have we lost the EGL context?
                        if (lostEglContext) {
                            stopEglSurfaceLocked();
                            stopEglContextLocked();
                            lostEglContext = false;
                        }

                        // When pausing, release the EGL surface:
                        if (pausing && mHaveEglSurface) {
                            stopEglSurfaceLocked();
                        }

                        // When pausing, optionally release the EGL Context:
                        if (pausing && mHaveEglContext) {
                            GLView view = mGLSurfaceViewWeakRef.get();
                            boolean preserveEglContextOnPause = view != null && view.mPreserveEGLContextOnPause;
                            if (!preserveEglContextOnPause || GLView.sGLThreadManager.shouldReleaseEGLContextWhenPausing()) {
                                stopEglContextLocked();
                            }
                        }

                        // When pausing, optionally terminate EGL:
                        if (pausing) {
                            if (GLView.sGLThreadManager.shouldTerminateEGLWhenPausing()) {
                                mEglHelper.finish();
                            }
                        }

                        // Have we lost the SurfaceView surface?
                        if ((!mHasSurface) && (!mWaitingForSurface)) {
                            if (mHaveEglSurface) {
                                stopEglSurfaceLocked();
                            }
                            mWaitingForSurface = true;
                            mSurfaceIsBad = false;
                            GLView.sGLThreadManager.notifyAll();
                        }

                        // Have we acquired the surface view surface?
                        if (mHasSurface && mWaitingForSurface) {
                            mWaitingForSurface = false;
                            GLView.sGLThreadManager.notifyAll();
                        }

                        if (doRenderNotification) {
                            wantRenderNotification = false;
                            doRenderNotification = false;
                            mRenderComplete = true;
                            GLView.sGLThreadManager.notifyAll();
                        }

                        // Ready to draw?
                        if (readyToDraw()) {

                            // If we don't have an EGL context, try to acquire one.
                            if (!mHaveEglContext) {
                                if (askedToReleaseEglContext) {
                                    askedToReleaseEglContext = false;
                                } else if (GLView.sGLThreadManager.tryAcquireEglContextLocked(this)) {
                                    try {
                                        mEglHelper.start();
                                    } catch (RuntimeException t) {
                                        GLView.sGLThreadManager.releaseEglContextLocked(this);
                                        throw t;
                                    }
                                    mHaveEglContext = true;
                                    createEglContext = true;

                                    GLView.sGLThreadManager.notifyAll();
                                }
                            }

                            if (mHaveEglContext && !mHaveEglSurface) {
                                mHaveEglSurface = true;
                                createEglSurface = true;
                                createGlInterface = true;
                                sizeChanged = true;
                            }

                            if (mHaveEglSurface) {
                                if (mSizeChanged) {
                                    sizeChanged = true;
                                    w = mWidth;
                                    h = mHeight;
                                    wantRenderNotification = true;

                                    // Destroy and recreate the EGL surface.
                                    createEglSurface = true;
                                    GLView view = mGLSurfaceViewWeakRef.get();
                                    if (view != null) {
                                        if (view.mRecorderController != null) {
                                            createRecordableSurface = true;
                                        }
                                        if (view.mImageReaderController != null) {
                                            createImageReaderSurface = true;
                                        }
                                    }
                                    mSizeChanged = false;
                                }
                                mRequestRender = false;
                                GLView.sGLThreadManager.notifyAll();
                                break;
                            }

                        }

                        // By design, this is the only place in a GLThread thread where we wait().
                        GLView.sGLThreadManager.wait();
                    } // end of inner looper
                } // end of synchronized(GLView.sGLThreadManager)

                if (event != null) {
                    event.run();
                    event = null;
                    continue;
                }

                if (createEglSurface) {
                    if (!mEglHelper.createSurface()) {
                        synchronized (GLView.sGLThreadManager) {
                            mSurfaceIsBad = true;
                            GLView.sGLThreadManager.notifyAll();
                        }
                        continue;
                    }
                    createEglSurface = false;
                }
                if (createRecordableSurface) {
                    mEglHelper.createRecordableSurface();
                    createRecordableSurface = false;
                }

                if (createImageReaderSurface) {
                    mEglHelper.createImageReaderSurface();
                    createImageReaderSurface = false;
                }
                if (!mEglHelper.makeCurrent(0)) {
                    synchronized (GLView.sGLThreadManager) {
                        mSurfaceIsBad = true;
                        GLView.sGLThreadManager.notifyAll();
                    }
                    continue;
                }

                if (createGlInterface) {
                    gl = (GL10) mEglHelper.createGL();

                    GLView.sGLThreadManager.checkGLDriver(gl);
                    createGlInterface = false;
                }

                if (createEglContext) {
                    GLView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onSurfaceCreated(gl, mEglHelper.mEglConfig);
                    }
                    createEglContext = false;
                }

                if (sizeChanged) {
                    GLView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        view.mRenderer.onSurfaceChanged(gl, w, h);
                    }
                    sizeChanged = false;
                }

                {
                    GLView view = mGLSurfaceViewWeakRef.get();
                    if (view != null) {
                        boolean result;
                        //result = mEglHelper.makeCurrent(0);
                        {
                            view.mRenderer.onDrawFrame(gl);
                            int swapError = mEglHelper.swap(0);
                            switch (swapError) {
                                case EGL10.EGL_SUCCESS:
                                    break;
                                case EGL11.EGL_CONTEXT_LOST:
                                    lostEglContext = true;
                                    break;
                                default:
                                    synchronized (GLView.sGLThreadManager) {
                                        mSurfaceIsBad = true;
                                        GLView.sGLThreadManager.notifyAll();
                                    }
                                    break;
                            }

                        }
                        if (view.mRecorderController != null && view.mRecorderController.isRecording()) {
                            result = mEglHelper.makeCurrent(1);
                            if (result) {
                                view.mRenderer.onDrawFrame(gl);
                                int swapError = mEglHelper.swap(1);
                                switch (swapError) {
                                    case EGL10.EGL_SUCCESS:
                                        break;
                                    case EGL11.EGL_CONTEXT_LOST:
                                        lostEglContext = true;
                                        break;
                                    default:
                                        synchronized (GLView.sGLThreadManager) {
                                            mSurfaceIsBad = true;
                                            GLView.sGLThreadManager.notifyAll();
                                        }
                                        break;
                                }

                            }
                        }
                        if (view.mImageReaderController != null) {
                            result = mEglHelper.makeCurrent(2);
                            if (result) {
                                view.mRenderer.onDrawFrame(gl);
                                int swapError = mEglHelper.swap(2);
                                switch (swapError) {
                                    case EGL10.EGL_SUCCESS:
                                        break;
                                    case EGL11.EGL_CONTEXT_LOST:
                                        lostEglContext = true;
                                        break;
                                    default:
                                        synchronized (GLView.sGLThreadManager) {
                                            mSurfaceIsBad = true;
                                            GLView.sGLThreadManager.notifyAll();
                                        }
                                        break;
                                }

                            }
                        }
                    }
                }

                if (wantRenderNotification) {
                    doRenderNotification = true;
                }
            } // end of outer looper

        } finally {
            /*
             * clean-up everything...
             */
            synchronized (GLView.sGLThreadManager) {
                stopEglSurfaceLocked();
                stopEglContextLocked();
            }
        }
    }

    public boolean ableToDraw() {
        return mHaveEglContext && mHaveEglSurface && readyToDraw();
    }

    private boolean readyToDraw() {
        return (!mPaused) && mHasSurface && (!mSurfaceIsBad)
                && (mWidth > 0) && (mHeight > 0)
                && (mRequestRender || (mRenderMode == GLView.RENDERMODE_CONTINUOUSLY));
    }

    public void setRenderMode(int renderMode) {
        if (!((GLView.RENDERMODE_WHEN_DIRTY <= renderMode) && (renderMode <= GLView.RENDERMODE_CONTINUOUSLY))) {
            throw new IllegalArgumentException("renderMode");
        }
        synchronized (GLView.sGLThreadManager) {
            mRenderMode = renderMode;
            GLView.sGLThreadManager.notifyAll();
        }
    }

    public int getRenderMode() {
        synchronized (GLView.sGLThreadManager) {
            return mRenderMode;
        }
    }

    public void requestRender() {
        synchronized (GLView.sGLThreadManager) {
            mRequestRender = true;
            GLView.sGLThreadManager.notifyAll();
        }
    }

    public void surfaceCreated() {
        synchronized (GLView.sGLThreadManager) {
            mHasSurface = true;
            GLView.sGLThreadManager.notifyAll();
            while ((mWaitingForSurface) && (!mExited)) {
                try {
                    GLView.sGLThreadManager.wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void surfaceDestroyed() {
        synchronized (GLView.sGLThreadManager) {
            mHasSurface = false;
            GLView.sGLThreadManager.notifyAll();
            while ((!mWaitingForSurface) && (!mExited)) {
                try {
                    GLView.sGLThreadManager.wait();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onPause() {
        synchronized (GLView.sGLThreadManager) {
            mRequestPaused = true;
            GLView.sGLThreadManager.notifyAll();
            while ((!mExited) && (!mPaused)) {
                try {
                    GLView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onResume() {
        synchronized (GLView.sGLThreadManager) {
            mRequestPaused = false;
            mRequestRender = true;
            mRenderComplete = false;
            GLView.sGLThreadManager.notifyAll();
            while ((!mExited) && mPaused && (!mRenderComplete)) {
                try {
                    GLView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void onWindowResize(int w, int h) {
        synchronized (GLView.sGLThreadManager) {
            mWidth = w;
            mHeight = h;
            mSizeChanged = true;
            mRequestRender = true;
            mRenderComplete = false;
            GLView.sGLThreadManager.notifyAll();

            // Wait for thread to react to resize and render a frame
            while (!mExited && !mPaused && !mRenderComplete && ableToDraw()) {
                try {
                    GLView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void requestExitAndWait() {
        // don't call this from GLThread thread or it is a guaranteed
        // deadlock!
        synchronized (GLView.sGLThreadManager) {
            mShouldExit = true;
            GLView.sGLThreadManager.notifyAll();
            while (!mExited) {
                try {
                    GLView.sGLThreadManager.wait();
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    public void requestReleaseEglContextLocked() {
        mShouldReleaseEglContext = true;
        GLView.sGLThreadManager.notifyAll();
    }

    /**
     * Queue an "event" to be run on the GL rendering thread.
     *
     * @param r the runnable to be run on the GL rendering thread.
     */
    public void queueEvent(Runnable r) {
        if (r == null) {
            throw new IllegalArgumentException("r must not be null");
        }
        synchronized (GLView.sGLThreadManager) {
            mEventQueue.add(r);
            GLView.sGLThreadManager.notifyAll();
        }
    }



    public void deleteRecordableSurface() {
        queueEvent(() -> mEglHelper.destroyRecorderSurface());
    }

    public void deleteImageReaderSurface() {
        queueEvent(() -> mEglHelper.destroyImageReaderSurface());
    }
}