INSTALLS += target qmlfiles
qmlfiles.files = $$OTHER_FILES

unix {
  qmlfiles.path = $$INSTALL_PREFIX/share/qt-components/$$TARGET
  target.path = $$INSTALL_PREFIX/bin
}
