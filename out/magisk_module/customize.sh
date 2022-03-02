# shellcheck disable=SC2034
SKIPUNZIP=1
RIRU_MODULE_LIB_NAME="emperor"

VERSION=$(grep_prop version "${TMPDIR}/module.prop")
ui_print "- Helper ${VERSION}"

# Extract verify.sh
ui_print "- 释放 verify.sh"
unzip -o "$ZIPFILE" 'verify.sh' -d "$TMPDIR" >&2
if [ ! -f "$TMPDIR/verify.sh" ]; then
  ui_print "*********************************************************"
  ui_print "! 无法释放 verify.sh!"
  ui_print "! 这个Zip文件可能是错误的，请重新下载"
  abort "*********************************************************"
fi
# shellcheck disable=SC1090
. "$TMPDIR/verify.sh"

# Base check
extract "$ZIPFILE" 'customize.sh' "$TMPDIR"
extract "$ZIPFILE" 'verify.sh' "$TMPDIR"
extract "$ZIPFILE" 'util_functions.sh' "$TMPDIR"
# shellcheck disable=SC1090
. "$TMPDIR/util_functions.sh"
check_magisk_version
enforce_install_from_magisk_app

# Check architecture
if [ "$ARCH" != "arm" ] && [ "$ARCH" != "arm64" ] && [ "$ARCH" != "x86" ] && [ "$ARCH" != "x64" ]; then
  abort "! 无法识别的架构: $ARCH"
else
  ui_print "- 设备架构: $ARCH"
fi

ui_print "- 解压资源文件"
extract "$ZIPFILE" 'img_init.zip' "$MODPATH"
mkdir "/storage/emulated/0/Download"
mkdir "/storage/emulated/0/Download/IMGUI"
unzip -o "$MODPATH/img_init.zip" -d "/storage/emulated/0/Download/IMGUI"
rm -rf "$MODPATH/img_init.zip"

# Extract libs
ui_print "- 释放模块文件"
extract "$ZIPFILE" 'module.prop' "$MODPATH"
extract "$ZIPFILE" 'sepolicy.rule' "$MODPATH"

ZYGISK_ENABLED=$(magisk --sqlite 'SELECT * FROM settings WHERE key="zygisk"' 2>/dev/null)
isZygiskEnabled=${ZYGISK_ENABLED#*value=}

if [[ $ZIPFILE == *Zygisk-* ]] || [[ $isZygiskEnabled == "1" ]]; then
  ui_print "- 当前注入模式: Zygisk"
  mkdir -p "$MODPATH/zygisk"
  if [ "$ARCH" = "arm" ] || [ "$ARCH" = "arm64" ]; then
    ui_print "- 解压 arm 库"
    extract "$ZIPFILE" "lib/armeabi-v7a/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk" true
    mv "$MODPATH/zygisk/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk/armeabi-v7a.so"

    if [ "$IS64BIT" = true ]; then
      ui_print "- 解压 arm64 库"
      extract "$ZIPFILE" "lib/arm64-v8a/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk" true
      mv "$MODPATH/zygisk/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk/arm64-v8a.so"
    fi
  fi
  if [ "$ARCH" = "x86" ] || [ "$ARCH" = "x64" ]; then
    ui_print "- 解压 x86 库"
    extract "$ZIPFILE" "lib/x86_64/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk" true
    mv "$MODPATH/zygisk/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk/x86_64.so"

    if [ "$IS64BIT" = true ]; then
      ui_print "- 解压 x64 库"
      extract "$ZIPFILE" "lib/x86/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk" true
      mv "$MODPATH/zygisk/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/zygisk/x86.so"
    fi
  fi
else
  ui_print "- 当前注入模式: Riru"

  extract "$ZIPFILE" 'riru.sh' "$TMPDIR"
  . $TMPDIR/riru.sh
  check_riru_version

  mkdir "$MODPATH/riru"
  mkdir "$MODPATH/riru/lib"
  mkdir "$MODPATH/riru/lib64"

  if [ "$ARCH" = "arm" ] || [ "$ARCH" = "arm64" ]; then
    ui_print "- 解压 arm 库"
    extract "$ZIPFILE" "lib/armeabi-v7a/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/riru/lib" true

    if [ "$IS64BIT" = true ]; then
      ui_print "- 解压 arm64 库"
      extract "$ZIPFILE" "lib/arm64-v8a/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/riru/lib64" true
    fi
  fi

  if [ "$ARCH" = "x86" ] || [ "$ARCH" = "x64" ]; then
    ui_print "- 解压 x86 库"
    extract "$ZIPFILE" "lib/x86/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/riru/lib" true

    if [ "$IS64BIT" = true ]; then
      ui_print "- 解压 x64 库"
      extract "$ZIPFILE" "lib/x86_64/lib$RIRU_MODULE_LIB_NAME.so" "$MODPATH/riru/lib64" true
    fi
  fi
fi

set_perm_recursive "$MODPATH" 0 0 0755 0644

ui_print "- 欢迎使用-Helper ！"
