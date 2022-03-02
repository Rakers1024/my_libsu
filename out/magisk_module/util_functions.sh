check_magisk_version() {
  ui_print "- Magisk 版本: $MAGISK_VER_CODE"
  if [ "$MAGISK_VER_CODE" -lt 23000 ]; then
    ui_print "*********************************************************"
    ui_print "! 请使用 Magisk v23+ 版本"
    abort    "*********************************************************"
  fi
}

enforce_install_from_magisk_app() {
  if $BOOTMODE; then
    ui_print "- Installing from Magisk app"
  else
    ui_print "*********************************************************"
    ui_print "! 该模块无法在 recovery 模式下安装"
    ui_print "! 在 recovery 模块下安装该模块，会出现一些未知的问题"
    ui_print "! 请从 Magisk app 中安装该模块"
    abort "*********************************************************"
  fi
}