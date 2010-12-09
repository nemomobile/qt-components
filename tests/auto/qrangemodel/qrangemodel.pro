include (../../tests.pri)

TEMPLATE = app
SOURCES += tst_qrangemodel.cpp
CONFIG += testcase

INSTALLS += target
unix {
  target.path = /opt/qt-components-tests/$$TARGET
}

include (../auto.pri)
