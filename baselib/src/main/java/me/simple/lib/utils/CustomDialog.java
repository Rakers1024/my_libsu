package me.simple.lib.utils;

import android.app.Activity;
import android.app.AppOpsManager;
import android.content.Context;
import android.graphics.drawable.GradientDrawable;
import android.os.Binder;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AlertDialog;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;

public class CustomDialog extends AlertDialog {

	private final Context mContext;

	private final ListView mListView;

	private int type = 0, ScreenHeight, ScreenWidth;

	public static int TYPE_FLOAT = 1;


	public CustomDialog(Context context) {
		super(context);
		mContext = context;
		DisplayMetrics dm = context.getResources().getDisplayMetrics();
		if (dm.heightPixels > dm.widthPixels) {
			ScreenHeight = dm.heightPixels;
			ScreenWidth = dm.widthPixels;
		} else {
			ScreenHeight = dm.widthPixels;
			ScreenWidth = dm.heightPixels;
		}
		mListView = new ListView(mContext);
		mListView.setDividerHeight(0);
		mListView.setDivider(null);
		mListView.setPadding(dp2px(8), dp2px(8), dp2px(8), dp2px(8));
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Window window = this.getWindow();

		GradientDrawable drawable = new GradientDrawable();
		getButton(BUTTON_POSITIVE).setTextColor(0xff000000);
		getButton(BUTTON_NEGATIVE).setTextColor(0xff000000);
		getButton(BUTTON_NEUTRAL).setTextColor(0xff000000);
		drawable.setShape(GradientDrawable.RECTANGLE);
		drawable.setColor(0xffffffff);
		drawable.setCornerRadii(new float[]{dp2px(8), dp2px(8), dp2px(8), dp2px(8), dp2px(8), dp2px(8), dp2px(8), dp2px(8)});
		window.setBackgroundDrawable(drawable);
		WindowManager.LayoutParams wlp = window.getAttributes();
		wlp.gravity = Gravity.CENTER;
		wlp.height = ViewGroup.LayoutParams.WRAP_CONTENT;
		wlp.width = (int) (ScreenWidth * 0.85);
		window.setAttributes(wlp);
	}

	private int dp2px(int dpVal) {
		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dpVal, mContext.getResources().getDisplayMetrics());
	}

	@RequiresApi(api = Build.VERSION_CODES.KITKAT)
	private boolean CheckOp() {
		try {
			AppOpsManager appOpsManager = (AppOpsManager) mContext.getSystemService(Context.APP_OPS_SERVICE);
			if (appOpsManager == null) {
				return false;
			}
			Class<? extends AppOpsManager> localClass = appOpsManager.getClass();
			Class[] arra = new Class[3];
			arra[0] = Integer.TYPE;
			arra[1] = Integer.TYPE;
			arra[2] = String.class;
			Method method = localClass.getMethod("checkOp", arra);
			if (method == null) {
				return false;
			}
			Object[] arra1 = new Object[3];
			arra1[0] = 24;
			arra1[1] = Binder.getCallingUid();
			arra1[2] = mContext.getPackageName();
			try {
				return (int) method.invoke(appOpsManager, arra1) == AppOpsManager.MODE_ALLOWED;
			} catch (InvocationTargetException | IllegalAccessException | IllegalArgumentException e) {
				e.printStackTrace();
				return false;
			}
		} catch (SecurityException | NoSuchMethodException e) {
			e.printStackTrace();
			return false;
		}
	}

	public void setItems(String[] items) {
		ArrayList<String> alist = new ArrayList<>(Arrays.asList(items));
		ArrayAdapter adp = new ArrayAdapter<>(mContext, android.R.layout.simple_list_item_1, alist);
		setAdapter(adp);
		mListView.setChoiceMode(ListView.CHOICE_MODE_NONE);
	}

	public void setOnItemClickListener(final AdapterView.OnItemClickListener listener) {
		mListView.setOnItemClickListener(listener);
    }

    public void setAdapter(ListAdapter adp) {
		setView(mListView);
        mListView.setAdapter(adp);
    }
    
    public void setType(int a) {
        type = a;
	}
    
	@RequiresApi(api = Build.VERSION_CODES.KITKAT)
	@Override
	public void show() {
		if (CheckOp() && type == TYPE_FLOAT) {
			Window window = this.getWindow();
			WindowManager.LayoutParams wlp = window.getAttributes();
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
				wlp.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
			} else {
				wlp.type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
			}
			RecorderFakeUtils.setFakeRecorderWindowLayoutParams(mContext, wlp);
			window.setAttributes(wlp);
		}

		if (mContext instanceof Activity){
			Activity mActivity = ((Activity)mContext);
			if (mActivity.isFinishing()){
				return;
			}
		}
		super.show();
	}

}

