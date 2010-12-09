INSTALLS += target qmlfiles
qmlfiles.files = $$OTHER_FILES

unix {
  qmlfiles.path = /opt/qt-components-tests/auto/$$TARGET
  target.path = /opt/qt-components-tests/auto/$$TARGET
}
