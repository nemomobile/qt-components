TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle

win32:DESTDIR = $$OUT_PWD

meego:include(../meego.pri)

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0xEB5F8C8C
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = ReadDeviceData SwEvent WriteDeviceData
    qmls.sources = *.qml
    DEPLOYMENT += qmls
}

SOURCES += main.cpp
OTHER_FILES += main.qml
