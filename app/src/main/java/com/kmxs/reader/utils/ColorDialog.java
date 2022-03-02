package com.kmxs.reader.utils;

import android.annotation.SuppressLint;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.graphics.drawable.GradientDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.kmxs.reader.R;

import es.dmoral.toasty.Toasty;
import me.simple.lib.utils.CustomDialog;

public class ColorDialog {

    
    public static void 控件圆角(View view, int InsideColor, float radiu) {
        GradientDrawable drawable = new GradientDrawable() ;
        drawable.setShape(GradientDrawable.RECTANGLE) ;
        drawable.setColor(InsideColor);
        drawable.setCornerRadii(new float[]{radiu,radiu,radiu,radiu,radiu,radiu,radiu,radiu});
        view.setBackgroundDrawable(drawable);
	}

	public static void show(final Context context) {
		@SuppressLint("InflateParams")
		final View view = LayoutInflater.from(context).inflate(R.layout.dialog_color, null, false);
        final TextView mmp6 = view.findViewById(R.id.mmp6);
		final LinearLayout mmp5 = view.findViewById(R.id.mmp5);
		final TextView mmp4 = view.findViewById(R.id.mmp4);
		final TextView mmp3 = view.findViewById(R.id.mmp3);
		final TextView mmp2 = view.findViewById(R.id.mmp2);
		final TextView mmp1 = view.findViewById(R.id.mmp1);

		SeekBar seek_Ap = view.findViewById(R.id.seek_Ap);
		SeekBar seek_red = view.findViewById(R.id.seek_red);
		SeekBar seek_green = view.findViewById(R.id.seek_green);
		SeekBar seek_blue = view.findViewById(R.id.seek_blue);

		seek_Ap.setMax(255);
		seek_Ap.setProgress(255);

		seek_red.setMax(255);
		seek_red.setProgress(1);

		seek_green.setMax(255);
		seek_green.setProgress(1);

		seek_blue.setMax(255);
		seek_blue.setProgress(1);

		seek_red.getProgressDrawable().setColorFilter(new PorterDuffColorFilter(0xFFFF0000, PorterDuff.Mode.SRC_ATOP));
		seek_red.getThumb().setColorFilter(new PorterDuffColorFilter(0xFFFF0000, PorterDuff.Mode.SRC_ATOP));

		seek_green.getProgressDrawable().setColorFilter(new PorterDuffColorFilter(0xFF00FF00, PorterDuff.Mode.SRC_ATOP));
		seek_green.getThumb().setColorFilter(new PorterDuffColorFilter(0xFF00FF00, PorterDuff.Mode.SRC_ATOP));

		seek_blue.getProgressDrawable().setColorFilter(new PorterDuffColorFilter(0xFF0000FF, PorterDuff.Mode.SRC_ATOP));
		seek_blue.getThumb().setColorFilter(new PorterDuffColorFilter(0xFF0000FF, PorterDuff.Mode.SRC_ATOP));

		控件圆角(mmp5, 0xFF000000, 360);

		seek_Ap.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

				@SuppressLint("SetTextI18n")
				public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                    progress = progress + 1;
					String e = Integer.toHexString(progress - 1).toUpperCase();
					if (e.length() == 1) {
						e = "0" + e;
						mmp6.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					} else {
						mmp6.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					}
                }

                public void onStartTrackingTouch(SeekBar seekBar) {

                }

                public void onStopTrackingTouch(SeekBar seekBar) {

                }
            });

		seek_red.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @SuppressLint("SetTextI18n")
				public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                    progress = progress + 1;
					String e = Integer.toHexString(progress - 1).toUpperCase();
					if (e.length() == 1) {
						e = "0" + e;
						mmp1.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					} else {
						mmp1.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					}
                }

                public void onStartTrackingTouch(SeekBar seekBar) {

                }

                public void onStopTrackingTouch(SeekBar seekBar) {

                }
            });

		seek_green.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @SuppressLint("SetTextI18n")
				public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                    progress = progress + 1;
					String e = Integer.toHexString(progress - 1).toUpperCase();
					if (e.length() == 1) {
						e = "0" + e;
						mmp2.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					} else {
						mmp2.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					}
                }

                public void onStartTrackingTouch(SeekBar seekBar) {

                }

                public void onStopTrackingTouch(SeekBar seekBar) {

                }
            });

		seek_blue.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @SuppressLint("SetTextI18n")
				public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                    progress = progress + 1;
					String e = Integer.toHexString(progress - 1).toUpperCase();
					if (e.length() == 1) {
						e = "0" + e;
						mmp3.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					} else {
						mmp3.setText(e);
						String d = mmp6.getText().toString() + mmp1.getText().toString() + mmp2.getText().toString() + mmp3.getText().toString();
						mmp4.setText("0x" + d);
						int ys = Color.parseColor("#" + d);
						控件圆角(mmp5, ys, 360);
					}
                }

                public void onStartTrackingTouch(SeekBar seekBar) {

                }

                public void onStopTrackingTouch(SeekBar seekBar) {

                }
            });


		final CustomDialog ColorDialog = new CustomDialog(context);
		ColorDialog.setTitle("调色板");
		ColorDialog.setView(view);
        ColorDialog.setType(CustomDialog.BUTTON_NEGATIVE);
		ColorDialog.setButton(DialogInterface.BUTTON_POSITIVE, "复制", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					String Color = mmp4.getText().toString();
					Color = Color.substring(2);
					ClipboardManager cm = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
					cm.setText(Color);
					String str = cm.getText().toString();
					if (Color.equals(str)) {
						Toasty.success(context, "复制成功！", Toast.LENGTH_SHORT).show();
					} else {
						Toasty.error(context, "复制失败！", Toast.LENGTH_SHORT).show();
					}
					dialog.dismiss();

				}
			});
		ColorDialog.setButton(DialogInterface.BUTTON_NEGATIVE, "取消", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int which) {
					dialog.dismiss();
				}
			});
		ColorDialog.show();
	}
}
