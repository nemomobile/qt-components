
DEFINES += Q_COMPONENTS_MEEGO
INCLUDEPATH += $$PWD

#contextsubscriber
{
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    INCLUDEPATH += /usr/include/contextsubscriber
    LIBS += -lcontextsubscriber
}

QT += dbus

SOURCES += \
    MeeGo/mdeclarativestatusbar.cpp \
    MeeGo/mdeclarativescreen.cpp

HEADERS += \
    MeeGo/mdeclarativestatusbar.h \
    MeeGo/mdeclarativescreen.h

QML_FILES = \
        MeeGo/qmldir \
        MeeGo/Window.qml


OTHER_FILES += $$QML_FILES
