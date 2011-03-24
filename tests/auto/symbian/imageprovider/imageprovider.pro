include(../../../tests.pri)

TARGET = tst_symbian_imageprovider
TEMPLATE = app
QT += declarative svg

SOURCES += tst_sdeclarativeimageprovider.cpp

OTHER_FILES += \
    tst_imageprovider.qml \
    list1.png

include(../../auto.pri)

