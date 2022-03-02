DATA_DIR=/data/user/0

function error() {
	exec echo "$@" 1>&2
}


echo "- 开始修改Magisk用户通知"
if [[ -z "$Magisk_Manager" ]]; then
	echo "- 开始识别Magisk Manger包名……"
	test -d $DATA_DIR/com.topjohnwu.magisk
		if [[ $? -eq 0 ]]; then
			Magisk_Manager=com.topjohnwu.magisk
		else
			echo -n "- 开始查找随机包名安装的Magisk Manger .\n"
			t=`ls /data/user_de/*/*/shared_prefs/su_timeout.xml 2>/dev/null`
				if [[ $? -eq 0 ]]; then
					echo -n "."
					Magisk_Manager=`echo $t | cut -d / -f 5`
				else
					echo -n " ."
					Magisk_Manager=$(magisk --sqlite "SELECT value FROM strings WHERE key='requester'" 2>/dev/null | cut -d= -f2)
					if [[ -z $Magisk_Manager ]]; then
						echo -n " ."
						Magisk_Manager=$(strings /data/adb/magisk.db | sed -rn 's/^.?requester//p')
						[[ -z $Magisk_Manager ]] && error "！未识别到Magisk Manger包名"
					fi
				fi
		fi
		echo -e "- 已识别到Magisk Manger包名为：$Magisk_Manager"
fi

[[ -d $DATA_DIR/$Magisk_Manager ]] || error "！未识别出Magisk Manger包名"

find /data/user_de/ -name "$Magisk_Manager" -type d | while read Dir; do
	user=`echo $Dir | cut -d / -f 4`
	File="$Dir/shared_prefs/${Magisk_Manager}_preferences.xml"
	
	am force-stop --user "$user" $Magisk_Manager

	! test -f "$File" && {
		echo "- 未检测到数据开始启动Magisk Manger初始化数据"
		s
		am start --user "$user" -n `dumpsys package $Magisk_Manager | fgrep -B 2 'android.intent.category.LAUNCHER' | awk '/\//{print $2}'`
		s
		input keyevent 4
		am force-stop --user "$user" $Magisk_Manager
	}
	
	! test -f "$File" && error "！加载数据出错" && continue
		echo "- 开始为$user用户修改用户通知"
		fgrep -q '<string name="su_notification">' "$File"
		if [[ $? -eq 0 ]]; then
		echo "- 正在编辑数据……"
		sed -i '/su_notification/c\	<string name="su_notification">0<\/string>' "$File"
		else
		echo "- 正在写入数据……"
		sed -i '/<\/map>/i\	<string name="su_notification">0<\/string>' "$File"
		fi
done
echo "- 修改完毕，执行Root命令即可看到效果"