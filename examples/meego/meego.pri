INSTALLS += target qmlfiles
qmlfiles.files = $$OTHER_FILES

unix {
  qmlfiles.path = /opt/qt-components/$$TARGET
  target.path = /opt/qt-components/$$TARGET
}
