TEMPLATE = app
TARGET = flickr
QT += network declarative

SOURCES += main.cpp
RESOURCES += flickr.qrc

OTHER_FILES += \
    qml/*.qml \
    qml/*.js \
    qml/images/* \
    *.svg
