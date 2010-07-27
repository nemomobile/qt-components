
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
    MeeGo/mdeclarativescreen.cpp \
    MeeGo/msnapshot.cpp

HEADERS += \
    MeeGo/mdeclarativestatusbar.h \
    MeeGo/mdeclarativescreen.h \
    MeeGo/msnapshot.h

QML_FILES = \
        MeeGo/qmldir \
        MeeGo/Window.qml


OTHER_FILES += $$QML_FILES
