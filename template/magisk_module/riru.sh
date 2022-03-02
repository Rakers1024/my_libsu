#!/sbin/sh
RIRU_MODULE_LIB_NAME="%%%RIRU_MODULE_LIB_NAME%%%"
RIRU_MODULE_ID_PRE24="%%%RIRU_MODULE_ID_PRE24%%%"

# Variables for customize.sh
RIRU_API=0
RIRU_MIN_COMPATIBLE_API=0
RIRU_VERSION_CODE=0
RIRU_VERSION_NAME=""

# Used by util_functions.sh
RIRU_MODULE_API_VERSION=%%%RIRU_MODULE_API_VERSION%%%
RIRU_MODULE_MIN_API_VERSION=%%%RIRU_MODULE_MIN_API_VERSION%%%
RIRU_MODULE_MIN_RIRU_VERSION_NAME="%%%RIRU_MODULE_MIN_RIRU_VERSION_NAME%%%"


if [ "$MAGISK_VER_CODE" -ge 21000 ]; then
  MAGISK_CURRENT_RIRU_MODULE_PATH=$(magisk --path)/.magisk/modules/riru-core
else
  MAGISK_CURRENT_RIRU_MODULE_PATH=/sbin/.magisk/modules/riru-core
fi

if [ ! -d $MAGISK_CURRENT_RIRU_MODULE_PATH ]; then
  ui_print "*********************************************************"
  ui_print "! Riru 未安装"
  ui_print "! 请先安装 Riru 后再使用"
  abort "*********************************************************"
fi

if [ -f "$MAGISK_CURRENT_RIRU_MODULE_PATH/disable" ] || [ -f "$MAGISK_CURRENT_RIRU_MODULE_PATH/remove" ]; then
  ui_print "*********************************************************"
  ui_print "! Riru 被禁用 或 即将移除"
  ui_print "! 请在 Magisk 中启用 Riru 模块"
  abort "*********************************************************"
fi

if [ -f $MAGISK_CURRENT_RIRU_MODULE_PATH/util_functions.sh ]; then
  ui_print "- 加载 $MAGISK_CURRENT_RIRU_MODULE_PATH/util_functions.sh"
  # shellcheck disable=SC1090
  . $MAGISK_CURRENT_RIRU_MODULE_PATH/util_functions.sh
else
  ui_print "*********************************************************"
  ui_print "! 当前 Riru 版本太低，该模块最少需要 Riru-$RIRU_MODULE_MIN_RIRU_VERSION_NAME"
  ui_print "! 请更新 Riru 后再使用"
  abort "*********************************************************"
fi
