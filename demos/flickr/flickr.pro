TEMPLATE = app
TARGET = flickr
QT += network declarative

SOURCES += main.cpp
RESOURCES += flickr.qrc

symbian {
    TARGET.UID3 = 0x2003DE91
    TARGET.EPOCHEAPSIZE = 0x400000 0x6000000 #64MB
    TARGET.CAPABILITY += NetworkServices
    ICON = flickr.svg
}

OTHER_FILES += \
    qml/*.qml \
    qml/*.js \
    qml/images/* \
    *.svg
