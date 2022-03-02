package com.kmxs.reader.activity;

import android.content.ClipboardManager;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.RequiresApi;

import com.kmxs.reader.App;
import com.kmxs.reader.R;

import me.simple.lib.activity.BaseActivity;
import me.simple.lib.widget.DialogFactory;

public class LogActivity extends BaseActivity {

	private Context mContext;

	@SuppressWarnings("deprecation")
	@RequiresApi(api = Build.VERSION_CODES.M)
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_log);
		mContext = this;

		LinearLayout status_bar = findViewById(R.id.status_bar);
		LinearLayout.LayoutParams Params = (LinearLayout.LayoutParams) status_bar.getLayoutParams();
		Params.height = App.getStatusBarHeight();
		status_bar.setLayoutParams(Params);

		//final Button 启动游戏 = findViewById(R.id.启动游戏);

		final TextView tv_log = findViewById(R.id.tv_log);
		tv_log.setText(getIntent().getStringExtra("CashStr"));

		LinearLayout bt_copy = findViewById(R.id.bt_copy);
		bt_copy.setOnClickListener(view -> {
			String log = tv_log.getText().toString();

			ClipboardManager cm = (ClipboardManager) mContext.getSystemService(Context.CLIPBOARD_SERVICE);
			cm.setText(log);
			String str = cm.getText().toString();
			String dialogMsg;
			if (log.equals(str)) {
				dialogMsg = "复制成功！赶快把神秘代码给作者吧！";
			} else {
				dialogMsg = "复制失败！请重新复制！";
			}
			DialogFactory.createTipsDialog(LogActivity.this, dialogMsg).show();
		});
	}

}
